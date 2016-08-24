#include "b_star_tree.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <utility>

using namespace std;

BStarTree::Node::Node(int macro_id)
    : macro_id_(macro_id),
      parent_id_(-1),
      left_child_id_(-1),
      right_child_id_(-1),
      is_visited_(false) {}

BStarTree::BStarTree(int num_nodes)
    : nodes_(num_nodes, Node(-1)), root_id_(-1) {
  assert(nodes_.size() == num_nodes);
  for (int i = 0; i < nodes_.size(); ++i) {
    nodes_[i].macro_id_ = i;
  }
  Skew();
}

int BStarTree::num_nodes() const { return nodes_.size(); }

int BStarTree::node_macro_id(int node_id) const {
  return nodes_.at(node_id).macro_id_;
}

void BStarTree::Print(int indent) const {
  const int num_spaces = 2;
  cout << string(num_spaces * indent, ' ') << "BStarTree:" << endl;
  cout << string(num_spaces * (indent + 1), ' ') << "root_id_: " << root_id_
       << endl;
  cout << string(num_spaces * (indent + 1), ' ') << "nodes_:" << endl;
  for (int i = 0; i < nodes_.size(); ++i) {
    cout << string(num_spaces * (indent + 2), ' ') << "Node: " << i << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "macro_id_: " << nodes_[i].macro_id_ << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "parent_id_: " << nodes_[i].parent_id_ << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "left_child_id_: " << nodes_[i].left_child_id_ << endl;
    cout << string(num_spaces * (indent + 3), ' ')
         << "right_child_id_: " << nodes_[i].right_child_id_ << endl;
  }
}

void BStarTree::Dfs(
    function<void(int current_node_id, int parent_id, bool is_from_left)>
        handler) {
  for (Node& node : nodes_) {
    node.is_visited_ = false;
  }
  stack<int> s;
  s.push(root_id_);
  handler(root_id_, -1, false);
  while (!s.empty()) {
    int current_node_id = s.top();
    Node& current_node = nodes_.at(current_node_id);
    int left_child_id = current_node.left_child_id_;
    int right_child_id = current_node.right_child_id_;
    if (left_child_id != -1 && !nodes_.at(left_child_id).is_visited_) {
      s.push(left_child_id);
      handler(left_child_id, current_node_id, true);
    } else if (right_child_id != -1 && !nodes_.at(right_child_id).is_visited_) {
      s.push(right_child_id);
      handler(right_child_id, current_node_id, false);
    } else {
      current_node.is_visited_ = true;
      s.pop();
    }
  }
}

void BStarTree::DeleteThenInsertNode(int node_id, int target_node_id) {
  DeleteNode(node_id);
  InsertNode(node_id, target_node_id);
}

void BStarTree::SwapNodes(int node_a_id, int node_b_id) {
  swap(nodes_.at(node_a_id).macro_id_, nodes_.at(node_b_id).macro_id_);
}

void BStarTree::Skew() {
  if (!nodes_.empty()) {
    root_id_ = 0;
    for (int i = 0; i < nodes_.size() - 1; ++i) {
      nodes_[i].left_child_id_ = i + 1;
    }
    for (int i = 1; i < nodes_.size(); ++i) {
      nodes_[i].parent_id_ = i - 1;
    }
  }
}

void BStarTree::DeleteNode(int node_id) {
  Node& node = nodes_.at(node_id);
  int parent_id = node.parent_id_;
  int left_child_id = node.left_child_id_;
  int right_child_id = node.right_child_id_;

  int child_id = -1;
  if (left_child_id != -1 && right_child_id != -1) {
    child_id = left_child_id;

    int current_id = child_id;
    while (nodes_.at(current_id).left_child_id_ != -1 &&
           nodes_.at(current_id).right_child_id_ != -1) {
      current_id = nodes_.at(current_id).left_child_id_;
    }
    Node& current_node = nodes_.at(current_id);
    if (current_node.right_child_id_ != -1) {
      current_node.left_child_id_ = current_node.right_child_id_;
      current_node.right_child_id_ = -1;
    }

    while (current_id != node_id) {
      Node& current_node = nodes_.at(current_id);
      Node& current_parent = nodes_.at(current_node.parent_id_);
      Node& current_parent_right_child =
          nodes_.at(current_parent.right_child_id_);
      current_node.right_child_id_ = current_parent.right_child_id_;
      current_parent_right_child.parent_id_ = current_id;
      current_id = current_node.parent_id_;
    }
  } else if (left_child_id != -1) {
    child_id = left_child_id;
  } else if (right_child_id != -1) {
    child_id = right_child_id;
  }

  if (child_id != -1) {
    nodes_.at(child_id).parent_id_ = parent_id;
  }

  if (parent_id == -1) {
    root_id_ = child_id;
  } else {
    Node& parent = nodes_.at(parent_id);
    if (parent.left_child_id_ == node_id) {
      parent.left_child_id_ = child_id;
    } else {
      parent.right_child_id_ = child_id;
    }
  }

  node.parent_id_ = -1;
  node.left_child_id_ = -1;
  node.right_child_id_ = -1;
}

// TODO: Let client decide where to insert.
void BStarTree::InsertNode(int node_id, int target_node_id) {
  Node& node = nodes_.at(node_id);
  Node& target_node = nodes_.at(target_node_id);

  node.parent_id_ = target_node_id;
  if (rand() % 2 == 0) {
    node.left_child_id_ = target_node.left_child_id_;
    if (node.left_child_id_ != -1) {
      nodes_.at(node.left_child_id_).parent_id_ = node_id;
    }
    target_node.left_child_id_ = node_id;
  } else {
    node.right_child_id_ = target_node.right_child_id_;
    if (node.right_child_id_ != -1) {
      nodes_.at(node.right_child_id_).parent_id_ = node_id;
    }
    target_node.right_child_id_ = node_id;
  }
}
