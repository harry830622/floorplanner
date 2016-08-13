#include "floorplan.hpp"

#include <cassert>
#include <stack>

using namespace std;

Floorplan::Floorplan(int num_macros) : width_(0), height_(0) {
  for (int i = 0; i < num_macros; ++i) {
    macro_instances_.push_back(MacroInstance(i));
    b_star_tree_.AddNewNode(i);
  }
  b_star_tree_.Skew();
}

int Floorplan::GetWidth() const { return width_; }

int Floorplan::GetHeight() const { return height_; }

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

void Floorplan::SetWidth(int width) { width_ = width; }

void Floorplan::SetHeight(int height) { height_ = height; }

void Floorplan::SetMacroInstanceX(int idx, int x) {
  macro_instances_.at(idx).x_ = x;
}

void Floorplan::SetMacroInstanceY(int idx, int y) {
  macro_instances_.at(idx).y_ = y;
}

void Floorplan::DfsBStarTree(
    function<void(int current_idx, int parent_idx, char position)> handler) {
  for (int i = 0; i < b_star_tree_.GetNumNodes(); ++i) {
    b_star_tree_.SetNodeIsVisited(i, false);
  }
  stack<int> s;
  int root_idx = b_star_tree_.GetRootNodeIdx();
  s.push(root_idx);
  handler(root_idx, -1, 'X');
  while (!s.empty()) {
    int current_idx = s.top();
    int left_child_idx = b_star_tree_.GetNodeLeftChildIdx(current_idx);
    int right_child_idx = b_star_tree_.GetNodeRightChildIdx(current_idx);
    if (left_child_idx != -1 &&
        !b_star_tree_.GetNodeIsVisited(left_child_idx)) {
      s.push(left_child_idx);
      handler(left_child_idx, current_idx, 'L');
    } else if (right_child_idx != -1 &&
               !b_star_tree_.GetNodeIsVisited(right_child_idx)) {
      s.push(right_child_idx);
      handler(right_child_idx, current_idx, 'R');
    } else {
      b_star_tree_.SetNodeIsVisited(current_idx, true);
      s.pop();
    }
  }
}

ostream& operator<<(ostream& os, const Floorplan& floorplan) {
  const int indent = 2;
  const BStarTree& b_star_tree = floorplan.GetBStarTree();
  os << "Floorplan:" << endl;
  os << string(indent, ' ') << "b_star_tree_:" << endl;
  os << string(indent * 2, ' ') << "root_idx_: " << b_star_tree.GetRootNodeIdx()
     << endl;
  os << string(indent * 2, ' ') << "nodes_:" << endl;
  for (int i = 0; i < b_star_tree.GetNumNodes(); ++i) {
    os << string(indent * 3, ' ') << "Node: " << i << endl;
    os << string(indent * 4, ' ')
       << "macro_instance_idx_: " << b_star_tree.GetNodeMacroInstanceIdx(i)
       << endl;
    os << string(indent * 4, ' ')
       << "parent_idx_: " << b_star_tree.GetNodeParentIdx(i) << endl;
    os << string(indent * 4, ' ')
       << "left_child_idx_: " << b_star_tree.GetNodeLeftChildIdx(i) << endl;
    os << string(indent * 4, ' ')
       << "right_child_idx_: " << b_star_tree.GetNodeRightChildIdx(i) << endl;
  }
  os << string(indent, ' ') << "macro_instances_:" << endl;
  for (int i = 0; i < floorplan.GetNumMacroInstances(); ++i) {
    os << string(indent * 2, ' ') << "MacroInstance: " << i << endl;
    os << string(indent * 3, ' ')
       << "macro_idx_: " << floorplan.GetMacroInstanceMacroIdx(i) << endl;
    os << string(indent * 3, ' ') << "x_: " << floorplan.GetMacroInstanceX(i)
       << endl;
    os << string(indent * 3, ' ') << "y_: " << floorplan.GetMacroInstanceY(i)
       << endl;
    os << string(indent * 3, ' ')
       << "is_rotated_: " << floorplan.GetMacroInstanceIsRotated(i) << endl;
  }

  return os;
}
