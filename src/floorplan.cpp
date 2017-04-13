#include "./floorplan.hpp"
#include "./contour.hpp"

#include <stack>

using namespace std;

Floorplan::Floorplan(int num_macros)
    : width_(0.0),
      height_(0.0),
      wirelength_(0.0),
      b_star_tree_(num_macros),
      macro_id_from_node_id_(num_macros),
      is_rotated_from_macro_id_(num_macros, false),
      macro_bounding_box_from_macro_id_(num_macros,
                                        make_pair(Point(0, 0), Point(0, 0))) {
  for (int i = 0; i < macro_id_from_node_id_.size(); ++i) {
    macro_id_from_node_id_[i] = i;
  }
}

double Floorplan::width() const { return width_; }

double Floorplan::height() const { return height_; }

double Floorplan::area() const { return width_ * height_; }

double Floorplan::wirelength() const { return wirelength_; }

const pair<Point, Point>& Floorplan::macro_bounding_box(int macro_id) const {
  return macro_bounding_box_from_macro_id_.at(macro_id);
}

void Floorplan::Perturb(const Database& database) {
  const int num_macros = is_rotated_from_macro_id_.size();
  const int num_nodes = num_macros;
  const int op = rand() % 3;
  switch (op) {
    case 0: {
      const int macro_id = [&]() {
        int id = rand() % num_macros;
        while (!database.macro(id).is_rotatable()) {
          id = rand() % num_macros;
        }
        return id;
      }();
      is_rotated_from_macro_id_.at(macro_id) =
          !is_rotated_from_macro_id_.at(macro_id);
      break;
    }
    case 1: {
      const int node_a_id = rand() % num_nodes;
      const int node_b_id = [&]() {
        int node_id = rand() % num_nodes;
        while (node_id == node_a_id) {
          node_id = rand() % num_nodes;
        }
        return node_id;
      }();
      swap(macro_id_from_node_id_.at(node_a_id),
           macro_id_from_node_id_.at(node_b_id));
      break;
    }
    case 2: {
      const int macro_a_id = rand() % num_macros;
      const int macro_b_id = [&]() {
        int macro_id = rand() % num_macros;
        while (macro_id == macro_a_id) {
          macro_id = rand() % num_macros;
        }
        return macro_id;
      }();
      const bool is_inserted_left = rand() % 2 == 0 ? true : false;
      b_star_tree_.DeleteAndInsert(macro_a_id, macro_b_id, is_inserted_left);
      break;
    }
    default:
      break;
  }
}

void Floorplan::Pack(const Database& database) {
  /* b_star_tree_.Print(); */
  b_star_tree_.UnvisitAll();
  const int root_id = b_star_tree_.root_id();
  const int root_macro_id = macro_id_from_node_id_.at(root_id);
  const Macro& root_macro = database.macro(root_macro_id);
  double root_macro_width = root_macro.width();
  double root_macro_height = root_macro.height();
  const bool is_root_macro_rotated =
      is_rotated_from_macro_id_.at(root_macro_id);
  if (is_root_macro_rotated) {
    swap(root_macro_width, root_macro_height);
  }
  Contour contour;
  /* contour.Print(); */
  tuple<Point, Point, list<Point>::iterator> t =
      contour.Update(0.0, root_macro_width, root_macro_height);
  /* contour.Print(); */
  macro_bounding_box_from_macro_id_.at(root_macro_id) =
      make_pair(get<0>(t), get<1>(t));
  list<Point>::iterator it_hint = get<2>(t);
  stack<int> unvisited_node_ids;
  unvisited_node_ids.push(root_id);
  while (!unvisited_node_ids.empty()) {
    const int current_node_id = unvisited_node_ids.top();
    const int current_macro_id = macro_id_from_node_id_.at(current_node_id);
    const pair<Point, Point> current_macro_bounding_box =
        macro_bounding_box_from_macro_id_.at(current_macro_id);
    const int left_child_id = b_star_tree_.left_child_id(current_node_id);
    const int right_child_id = b_star_tree_.right_child_id(current_node_id);
    if (left_child_id != -1 && !b_star_tree_.is_visited(left_child_id)) {
      unvisited_node_ids.push(left_child_id);
      const int left_child_macro_id = macro_id_from_node_id_.at(left_child_id);
      const Macro& left_child_macro = database.macro(left_child_macro_id);
      double left_child_macro_width = left_child_macro.width();
      double left_child_macro_height = left_child_macro.height();
      const bool is_left_child_macro_rotated =
          is_rotated_from_macro_id_.at(left_child_macro_id);
      if (is_left_child_macro_rotated) {
        swap(left_child_macro_width, left_child_macro_height);
      }
      /* t = contour.Update(current_macro_bounding_box.second.x(), */
      /*                    left_child_macro_width, left_child_macro_height, */
      /*                    it_hint); */
      t = contour.Update(current_macro_bounding_box.second.x(),
                         left_child_macro_width, left_child_macro_height);
      /* contour.Print(); */
      macro_bounding_box_from_macro_id_.at(left_child_macro_id) =
          make_pair(get<0>(t), get<1>(t));
      it_hint = get<2>(t);
    } else if (right_child_id != -1 &&
               !b_star_tree_.is_visited(right_child_id)) {
      unvisited_node_ids.push(right_child_id);
      const int right_child_macro_id =
          macro_id_from_node_id_.at(right_child_id);
      const Macro& right_child_macro = database.macro(right_child_macro_id);
      double right_child_macro_width = right_child_macro.width();
      double right_child_macro_height = right_child_macro.height();
      const bool is_right_child_macro_rotated =
          is_rotated_from_macro_id_.at(right_child_macro_id);
      if (is_right_child_macro_rotated) {
        swap(right_child_macro_width, right_child_macro_height);
      }
      /* t = contour.Update(current_macro_bounding_box.first.x(), */
      /*                    right_child_macro_width, right_child_macro_height, */
      /*                    it_hint); */
      t = contour.Update(current_macro_bounding_box.first.x(),
                         right_child_macro_width, right_child_macro_height);
      /* contour.Print(); */
      macro_bounding_box_from_macro_id_.at(right_child_macro_id) =
          make_pair(get<0>(t), get<1>(t));
      it_hint = get<2>(t);
    } else {
      unvisited_node_ids.pop();
      b_star_tree_.Visit(current_node_id);
    }
  }
  width_ = contour.max_x();
  height_ = contour.max_y();
  for (int i = 0; i < database.num_nets(); ++i) {
    wirelength_ +=
        database.net(i).ComputeWirelength(macro_bounding_box_from_macro_id_);
  }
  /* contour.Print(); */
  /* cout << contour.max_x() << " " << contour.max_y() << endl; */
  /* cout << "Boxs:" << endl; */
  /* for (const auto& box : macro_bounding_box_from_macro_id_) { */
  /*   box.first.Print(cout, 1); */
  /*   box.second.Print(cout, 1); */
  /* } */
}
