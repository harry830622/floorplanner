#include "floorplan.hpp"

#include <cassert>

using namespace std;

Floorplan::Floorplan(int num_macros) {
  for (int i = 0; i < num_macros; ++i) {
    macro_instances_.push_back(MacroInstance(i));
    b_star_tree_.AddNewNode(i);
  }
  b_star_tree_.Skew();
}

const BStarTree& Floorplan::GetBStarTree() const { return b_star_tree_; }

int Floorplan::GetNumMacroInstances() const { return macro_instances_.size(); }

int Floorplan::GetMacroInstanceMacroIdx(int idx) const {
  return macro_instances_.at(idx).macro_idx_;
}

int Floorplan::GetMacroInstanceX(int idx) const {
  return macro_instances_.at(idx).x_;
}

int Floorplan::GetMacroInstanceY(int idx) const {
  return macro_instances_.at(idx).y_;
}

int Floorplan::GetMacroInstanceIsRotated(int idx) const {
  return macro_instances_.at(idx).is_rotated_;
}

int Floorplan::CalculateBestWidth() const {}
int Floorplan::CalculateBestHeight() const {}

Floorplan Floorplan::Perturb() const {
  Floorplan perturbed_floorplan(*this);
  int num_macro_instances = perturbed_floorplan.macro_instances_.size();
  BStarTree& perturbed_b_star_tree = perturbed_floorplan.b_star_tree_;
  int num_nodes = perturbed_b_star_tree.GetNumNodes();

  assert(num_nodes == num_macro_instances);

  int op = rand() % 3;
  switch (op) {
    case 0: {
      int idx = rand() % num_macro_instances;
      MacroInstance& macro_instance =
          perturbed_floorplan.macro_instances_.at(idx);
      macro_instance.is_rotated_ = !macro_instance.is_rotated_;
      break;
    }
    case 1: {
      int idx_a = rand() % num_nodes;
      int idx_b = rand() % num_nodes;
      while (idx_a == idx_b) {
        idx_b = rand() % num_nodes;
      }
      perturbed_b_star_tree.DeleteNode(idx_a);
      perturbed_b_star_tree.InsertNode(idx_a, idx_b,
                                       rand() % 100 < 50 ? 'L' : 'R');
      break;
    }
    case 2: {
      int idx_a = rand() % num_nodes;
      int idx_b = rand() % num_nodes;
      while (idx_a == idx_b) {
        idx_b = rand() % num_nodes;
      }
      perturbed_b_star_tree.SwapNodes(idx_a, idx_b);
      break;
    }
    default:
      break;
  }

  return perturbed_floorplan;
}

void Floorplan::SetMacroInstanceX(int idx, int x) {
  macro_instances_.at(idx).x_ = x;
}

void Floorplan::SetMacroInstanceY(int idx, int y) {
  macro_instances_.at(idx).y_ = y;
}

ostream& operator<<(ostream& os, const Floorplan& floorplan) {
  const int indent = 2;
  const BStarTree& b_star_tree = floorplan.GetBStarTree();
  cout << "Floorplan:" << endl;
  cout << string(indent, ' ') << "b_star_tree_:" << endl;
  cout << string(indent * 2, ' ')
       << "root_idx_: " << b_star_tree.GetRootNodeIdx() << endl;
  cout << string(indent * 2, ' ') << "nodes_:" << endl;
  for (int i = 0; i < b_star_tree.GetNumNodes(); ++i) {
    cout << string(indent * 3, ' ') << "Node: " << i << endl;
    cout << string(indent * 4, ' ')
         << "macro_instance_idx_: " << b_star_tree.GetNodeMacroInstanceIdx(i)
         << endl;
    cout << string(indent * 4, ' ')
         << "parent_idx_: " << b_star_tree.GetNodeParentIdx(i) << endl;
    cout << string(indent * 4, ' ')
         << "left_child_idx_: " << b_star_tree.GetNodeLeftChildIdx(i) << endl;
    cout << string(indent * 4, ' ')
         << "right_child_idx_: " << b_star_tree.GetNodeRightChildIdx(i) << endl;
  }
  cout << string(indent, ' ') << "macro_instances_:" << endl;
  for (int i = 0; i < floorplan.GetNumMacroInstances(); ++i) {
    cout << string(indent * 2, ' ') << "MacroInstance: " << i << endl;
    cout << string(indent * 3, ' ')
         << "macro_idx_: " << floorplan.GetMacroInstanceMacroIdx(i) << endl;
    cout << string(indent * 3, ' ') << "x_: " << floorplan.GetMacroInstanceX(i)
         << endl;
    cout << string(indent * 3, ' ') << "y_: " << floorplan.GetMacroInstanceY(i)
         << endl;
    cout << string(indent * 3, ' ')
         << "is_rotated_: " << floorplan.GetMacroInstanceIsRotated(i) << endl;
  }

  return os;
}
