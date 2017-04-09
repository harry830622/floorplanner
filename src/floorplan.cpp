#include "./floorplan.hpp"

using namespace std;

Floorplan::Floorplan(int num_macros)
    : area_(0.0),
      wirelength_(0.0),
      b_star_tree_(num_macros),
      macro_id_from_node_id_(num_macros),
      is_rotated_from_macro_id_(num_macros, false),
      macro_coordinates_from_macro_id_(num_macros, {Point(0, 0), Point(0, 0)}) {
  for (int i = 0; i < macro_id_from_node_id_.size(); ++i) {
    macro_id_from_node_id_[i] = i;
  }
}

double Floorplan::area() const { return area_; }

double Floorplan::wirelength() const { return wirelength_; }

void Floorplan::Perturb() {
  const int num_macros = is_rotated_from_macro_id_.size();
  const int num_nodes = num_macros;
  const int op = rand() % 3;
  if (op == 0) {
    const int macro_id = rand() % num_macros;
    is_rotated_from_macro_id_.at(macro_id) =
        !is_rotated_from_macro_id_.at(macro_id);
  } else if (op == 1) {
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
  } else {
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
  }
}

void Floorplan::Pack(const Database& database) {}
