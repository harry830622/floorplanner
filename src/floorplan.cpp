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
      macro_coordinates_from_macro_id_(num_macros, {Point(0, 0), Point(0, 0)}) {
  for (int i = 0; i < macro_id_from_node_id_.size(); ++i) {
    macro_id_from_node_id_[i] = i;
  }
}

double Floorplan::width() const { return width_; }

double Floorplan::height() const { return height_; }

double Floorplan::area() const { return width_ * height_; }

double Floorplan::wirelength() const { return wirelength_; }

void Floorplan::Perturb() {
  const int num_macros = is_rotated_from_macro_id_.size();
  const int num_nodes = num_macros;
  const int op = rand() % 3;
  switch (op) {
    case 0: {
      const int macro_id = rand() % num_macros;
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
  stack<int> unvisited_node_ids;
  unvisited_node_ids.push(root_id);
  Contour contour;
  // TODO
  contour.Update(0.0, root_macro_width, root_macro_height);
}
