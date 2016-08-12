#include "b_star_tree.hpp"

using namespace std;

int BStarTree::GetRootIdx() const {
  return root_idx_;
}

int BStarTree::GetNumNodes() const {
  return nodes_.size();
}

int BStarTree::GetNodeMacroIdx(int idx) const {
  return nodes_.at(idx).macro_idx_;
}

int BStarTree::GetNodeParentIdx(int idx) const {
  return nodes_.at(idx).parent_idx_;
}

int BStarTree::GetNodeLeftChildIdx(int idx) const {
  return nodes_.at(idx).left_child_idx_;
}

int BStarTree::GetNodeRightChildIdx(int idx) const {
  return nodes_.at(idx).right_child_idx_;
}

int BStarTree::GetNodeX(int idx) const {
  return nodes_.at(idx).x_;
}

int BStarTree::GetNodeY(int idx) const {
  return nodes_.at(idx).y_;
}

bool BStarTree::GetNodeIsRotated(int idx) const {
  return nodes_.at(idx).is_rotated_;
}

int BStarTree::CalculateHeight() const {
  return Height(root_idx_);
}

int BStarTree::CalculateChipWidth() const {
}

int BStarTree::CalculateChipHeight() const {
}

BStarTree BStarTree::Perturb() const {
}

void BStarTree::SetNodeX(int idx, int x) {
}

void BStarTree::SetNodeY(int idx, int y) {
}

int BStarTree::AddNewNode(int macro_idx) {
  int idx = nodes_.size();

  nodes_.push_back(Node(macro_idx));

  return idx;
}

void BStarTree::Skew() {
  root_idx_ = 0;
  for (int i = 0; i < nodes_.size() - 1; ++i) {
    nodes_[i].left_child_idx_ = i + 1;
  }
  for (int i = 1; i < nodes_.size(); ++i) {
    nodes_[i].parent_idx_ = i - 1;
  }
}

void BStarTree::UpdateContour(int x, int width, int height) {
}

int BStarTree::Height(int root_idx) const {
  if (root_idx == -1) {
    return 0;
  }

  int left_child_idx = nodes_.at(root_idx).left_child_idx_;
  int right_child_idx = nodes_.at(root_idx).right_child_idx_;

  return 1 + max(Height(left_child_idx), Height(right_child_idx));
}

void BStarTree::Rotate(int node_idx) {
}

void BStarTree::Swap(int node_a_idx, int node_b_idx) {
}

void BStarTree::Delete(int node_idx) {
}

void BStarTree::Insert(int node_idx) {
}

std::ostream& operator<<(std::ostream& os, const BStarTree& b_star_tree) {
  const int indent = 2;
  cout << string(indent, ' ') << "root_idx_: " << b_star_tree.GetRootIdx()
       << endl;
  cout << string(indent, ' ') << "nodes_:" << endl;
  for (int i = 0; i < b_star_tree.GetNumNodes(); ++i) {
    cout << string(indent * 2, ' ') << "Node: " << i << endl;
    cout << string(indent * 3, ' ')
         << "parent_idx_: " << b_star_tree.GetNodeParentIdx(i) << endl;
    cout << string(indent * 3, ' ')
         << "left_child_idx_: " << b_star_tree.GetNodeLeftChildIdx(i) << endl;
    cout << string(indent * 3, ' ')
         << "right_child_idx_: " << b_star_tree.GetNodeRightChildIdx(i) << endl;
    cout << string(indent * 3, ' ') << "x_: " << b_star_tree.GetNodeX(i)
         << endl;
    cout << string(indent * 3, ' ') << "y_: " << b_star_tree.GetNodeY(i)
         << endl;
    cout << string(indent * 3, ' ')
         << "is_rotated_: " << b_star_tree.GetNodeIsRotated(i) << endl;
  }
  return os;
}
