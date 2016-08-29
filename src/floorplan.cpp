#include "floorplan.hpp"

#include "contour.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using helpers::Point;
using helpers::Rect;
using uni_database::UniDatabase;
using uni_database::Keys;
using namespace std;

Floorplan::Macro::Macro(int data_id)
    : data_id_(data_id), coordinate_(0, 0), is_rotated_(false) {}

Floorplan::Net::Net(int data_id)
    : data_id_(data_id),
      bounding_box_(helpers::Point<int>(std::numeric_limits<int>::max(),
                                        std::numeric_limits<int>::max()),
                    helpers::Point<int>(0, 0)) {}

Floorplan::Floorplan(const UniDatabase& database)
    : width_(0),
      height_(0),
      average_area_(0.0),
      average_wire_length_(0.0),
      average_uphill_cost_(0.0) {
  map<int, int> macro_ids;
  int num_macros = database.Iterate(
      Keys{"macros"},
      [&, this](const string& key, int child_id) {
        macro_ids.insert(make_pair(child_id, this->macros_.size()));
        this->macros_.push_back(Macro(child_id));
      });

  b_star_tree_ = BStarTree(num_macros);

  database.Iterate(Keys{"nets"},
                   [this](const string& key, int child_id) {
                     this->nets_.push_back(Net(child_id));
                   });
  for (Net& net : nets_) {
    if (database.Id(net.data_id_, "terminals") != -1) {
      int min_x = numeric_limits<int>::max();
      int min_y = numeric_limits<int>::max();
      int max_x = 0;
      int max_y = 0;
      database.Iterate(net.data_id_, "terminals",
                       [&](const string& key, int child_id) {
                         int terminal_id = database.data<int>(child_id);
                         int x = database.Data<int>(terminal_id, "x");
                         int y = database.Data<int>(terminal_id, "y");
                         min_x = min(x, min_x);
                         min_y = min(y, min_y);
                         max_x = max(x, max_x);
                         max_y = max(y, max_y);
                       });
      net.bounding_box_ =
          Rect<int>(Point<int>(min_x, min_y), Point<int>(max_x, max_y));
    }
    if (database.Id(net.data_id_, "macros") != -1) {
      database.Iterate(
          net.data_id_, "macros", [&](const string& key, int child_id) {
            int macro_id = macro_ids.at(database.data<int>(child_id));
            net.macro_ids_.push_back(macro_id);
          });
    }
  }

  Pack(database);
  const int num_perturbations = 10000;
  long long total_area = 0;
  double total_wire_length = 0.0;
  for (int i = 0; i < num_perturbations; ++i) {
    Perturb();
    Pack(database);
    total_area += (width_ * height_);
    total_wire_length += WireLength(database);
  }
  average_area_ = total_area / static_cast<double>(num_perturbations);
  average_wire_length_ = total_wire_length / num_perturbations;

  double total_uphill_cost = 0.0;
  double last_cost = Cost(database);
  for (int i = 0; i < num_perturbations; ++i) {
    Perturb();
    Pack(database);
    double cost = Cost(database);
    double cost_delta = cost - last_cost;
    if (cost_delta > 0) {
      total_uphill_cost += cost;
    }
    last_cost = cost;
  }
  average_uphill_cost_ = total_uphill_cost / num_perturbations;
}

int Floorplan::width() const { return width_; }

int Floorplan::height() const { return height_; }

int Floorplan::num_macros() const { return macros_.size(); }

double Floorplan::average_uphill_cost() const { return average_uphill_cost_; }

void Floorplan::Print(int indent) const {
  const int num_spaces = 2;
  cout << string(num_spaces * indent, ' ') << "Floorplan:" << endl;
  cout << string(num_spaces * (indent + 1), ' ') << "macros_:" << endl;
  for (int i = 0; i < macros_.size(); ++i) {
    cout << string(num_spaces * (indent + 2), ' ') << "Macro: " << i << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "data_id_: " << macros_[i].data_id_ << endl;
    cout << string(num_spaces * (indent + 3), ' ') << "coordinate_: "
         << "(" << macros_[i].coordinate_.x() << ", "
         << macros_[i].coordinate_.y() << ")" << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "is_rotated_: " << macros_[i].is_rotated_ << endl;
  }
  cout << string(num_spaces * (indent + 1), ' ') << "nets_:" << endl;
  for (int i = 0; i < nets_.size(); ++i) {
    cout << string(num_spaces * (indent + 2), ' ') << "Net: " << i << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "data_id_: " << nets_[i].data_id_ << endl;
    cout << string(num_spaces * (indent + 3), ' ') << "bounding_box_:" << endl;
    cout << string(num_spaces * (indent + 4), ' ') << "lower_left_: "
         << "(" << nets_[i].bounding_box_.lower_left().x() << ", "
         << nets_[i].bounding_box_.lower_left().y() << ")" << endl;
    cout << string(num_spaces * (indent + 4), ' ') << "upper_right: "
         << "(" << nets_[i].bounding_box_.upper_right().x() << ", "
         << nets_[i].bounding_box_.upper_right().y() << ")" << endl;
    cout << string(num_spaces * (indent + 3), ' ') << "macro_ids_: ";
    for (int macro_id : nets_[i].macro_ids_) {
      cout << macro_id << " ";
    }
    cout << endl;
  }
  b_star_tree_.Print(indent + 1);
}

int Floorplan::IterateMacros(
    function<void(int data_id, const Point<int>& coordinate, bool is_rotated)>
        handler) const {
  for (const Macro& macro : macros_) {
    handler(macro.data_id_, macro.coordinate_, macro.is_rotated_);
  }
  return macros_.size();
}

double Floorplan::WireLength(const UniDatabase& database) const {
  double wire_length = 0.0;
  for (const Net& net : nets_) {
    double min_x = static_cast<double>(net.bounding_box_.lower_left().x());
    double min_y = static_cast<double>(net.bounding_box_.lower_left().y());
    double max_x = static_cast<double>(net.bounding_box_.upper_right().x());
    double max_y = static_cast<double>(net.bounding_box_.upper_right().y());
    for (int macro_id : net.macro_ids_) {
      const Macro& macro = macros_.at(macro_id);
      int width = database.Data<int>(macro.data_id_, "width");
      int height = database.Data<int>(macro.data_id_, "height");
      bool is_rotated = macro.is_rotated_;
      if (is_rotated) {
        swap(width, height);
      }
      double x = macro.coordinate_.x() + width / 2.0;
      double y = macro.coordinate_.y() + height / 2.0;
      min_x = min(x, min_x);
      min_y = min(y, min_y);
      max_x = max(x, max_x);
      max_y = max(y, max_y);
    }
    wire_length += (max_x - min_x + max_y - min_y);
  }
  return wire_length;
}

double Floorplan::Cost(const UniDatabase& database, double alpha) const {
  const int outline_width = database.Data<int>(Keys{"outline", "width"});
  const int outline_height = database.Data<int>(Keys{"outline", "height"});

  double penalty = 0.0;
  if (width_ > outline_width || height_ > outline_height) {
    if (width_ > outline_width && height_ > outline_height) {
      penalty += (width_ * height_ - outline_width * outline_height);
    } else if (width_ > outline_width) {
      penalty += ((width_ - outline_width) * height_);
      /* penalty += ((width_ - outline_width) * height_ + */
      /*             outline_width * (outline_height - height_)); */
    } else if (height_ > outline_height) {
      penalty += (width_ * (height_ - outline_height));
      /* penalty += (width_ * (height_ - outline_height) + */
      /*             (outline_width - width_) * outline_height); */
    }
    penalty += ((width_ - outline_width) * (width_ - outline_width) +
                (height_ - outline_height) * (height_ - outline_height));
    /* double ratio = height_ / static_cast<double>(width_); */
    /* double outline_ratio = outline_height / static_cast<double>(outline_width); */
    /* penalty += ((ratio - outline_ratio) * (ratio - outline_ratio)); */
  }

  const int area = width_ * height_;
  const double wire_length = WireLength(database);
  const int penalty_weight = 10;
  double cost = alpha * area / average_area_ +
                (1 - alpha) * wire_length / average_wire_length_ +
                penalty_weight * penalty / average_area_;

  /* cout << alpha * area / average_area_ << " " */
  /*      << (1 - alpha) * wire_length / average_wire_length_ << " " */
  /*      << penalty_weight * penalty << endl; */

  return cost;
}

void Floorplan::Perturb() {
  int num_macros = macros_.size();
  int num_nodes = b_star_tree_.num_nodes();

  assert(num_nodes == num_macros);

  int op = rand() % 3;
  switch (op) {
    case 0: {
      int id = rand() % num_macros;
      Macro& macro = macros_.at(id);
      macro.is_rotated_ = !macro.is_rotated_;
      break;
    }
    case 1: {
      int id_a = rand() % num_nodes;
      int id_b = rand() % num_nodes;
      while (id_a == id_b) {
        id_b = rand() % num_nodes;
      }
      b_star_tree_.DeleteThenInsertNode(id_a, id_b);
      break;
    }
    case 2: {
      int id_a = rand() % num_nodes;
      int id_b = rand() % num_nodes;
      while (id_a == id_b) {
        id_b = rand() % num_nodes;
      }
      b_star_tree_.SwapNodes(id_a, id_b);
      break;
    }
    default:
      break;
  }
}

void Floorplan::Pack(const UniDatabase& database) {
  Contour contour;
  b_star_tree_.Dfs([&](int current_node_id, int parent_id, bool is_from_left) {
    int macro_id = b_star_tree_.node_macro_id(current_node_id);
    Macro& macro = macros_.at(macro_id);
    int macro_data_id = macro.data_id_;
    int macro_width = database.Data<int>(macro_data_id, "width");
    int macro_height = database.Data<int>(macro_data_id, "height");
    bool is_macro_rotated = macro.is_rotated_;
    if (is_macro_rotated) {
      swap(macro_width, macro_height);
    }

    if (parent_id == -1) {
      macro.coordinate_.set_x(0);
      macro.coordinate_.set_y(0);
      contour.Update(0, macro_width, macro_height);
    } else {
      int parent_macro_id = b_star_tree_.node_macro_id(parent_id);
      const Macro& parent_macro = macros_.at(parent_macro_id);
      int parent_macro_data_id = parent_macro.data_id_;
      int parent_macro_x = parent_macro.coordinate_.x();
      int parent_macro_width =
          database.Data<int>(parent_macro_data_id, "width");
      bool is_parent_macro_rotated = parent_macro.is_rotated_;
      if (is_parent_macro_rotated) {
        parent_macro_width = database.Data<int>(parent_macro_data_id, "height");
      }
      if (is_from_left) {
        int x = parent_macro_x + parent_macro_width;
        macro.coordinate_.set_x(x);
        macro.coordinate_.set_y(contour.MaxYBetween(x, x + macro_width));
        contour.Update(x, macro_width, macro_height);
      } else {
        int x = parent_macro_x;
        macro.coordinate_.set_x(x);
        macro.coordinate_.set_y(contour.MaxYBetween(x, x + macro_width));
        contour.Update(x, macro_width, macro_height);
      }
    }
  });
  width_ = contour.max_x();
  height_ = contour.MaxY();
}
