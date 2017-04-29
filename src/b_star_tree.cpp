#include "./b_star_tree.hpp"

#include <stack>

using namespace std;

BStarTree::BStarTree(int num_macros)
    : root_id_(0), nodes_(num_macros, Node(-1, -1, -1)) {
  const int num_nodes = nodes_.size();

  for (int i = 0; i < num_nodes - 1; ++i) {
    nodes_[i].left_child_id_ = i + 1;
  }
  for (int i = 1; i < num_nodes; ++i) {
    nodes_[i].parent_id_ = i - 1;
  }

  /* for (int i = 0; i < num_nodes / 2 + 1; ++i) { */
  /*   const int right_child_id = (i + 1) * 2; */
  /*   const int left_child_id = right_child_id - 1; */
  /*   if (left_child_id < num_nodes) { */
  /*     nodes_[i].left_child_id_ = left_child_id; */
  /*   } */
  /*   if (right_child_id < num_nodes) { */
  /*     nodes_[i].right_child_id_ = right_child_id; */
  /*   } */
  /* } */
  /* for (int i = 1; i < num_nodes; ++i) { */
  /*   nodes_[i].parent_id_ = (i - 1) / 2; */
  /* } */
}

void BStarTree::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "BStarTree:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "root_id_:" << root_id_
     << endl;
  os << string(indent_level * indent_size, ' ') << "nodes_:" << endl;
  ++indent_level;
  for (int i = 0; i < nodes_.size(); ++i) {
    const Node& node = nodes_[i];
    os << string(indent_level * indent_size, ' ') << "Node: " << i << endl;
    ++indent_level;
    os << string(indent_level * indent_size, ' ')
       << "parent_id_: " << node.parent_id_ << endl;
    os << string(indent_level * indent_size, ' ')
       << "left_child_id_: " << node.left_child_id_ << endl;
    os << string(indent_level * indent_size, ' ')
       << "right_child_id_: " << node.right_child_id_ << endl;
    --indent_level;
  }
}

int BStarTree::num_nodes() const { return nodes_.size(); }

int BStarTree::root_id() const { return root_id_; }

int BStarTree::parent_id(int node_id) const { return node(node_id).parent_id_; }

int BStarTree::left_child_id(int node_id) const {
  return node(node_id).left_child_id_;
}
int BStarTree::right_child_id(int node_id) const {
  return node(node_id).right_child_id_;
}

bool BStarTree::is_visited(int node_id) const {
  return node(node_id).is_visited_;
}

void BStarTree::Visit(int node_id) { node(node_id).is_visited_ = true; }

void BStarTree::UnvisitAll() {
  for (Node& node : nodes_) {
    node.is_visited_ = false;
  }
}

void BStarTree::DeleteAndInsert(int deleted_node_id, int target_node_id,
                                pair<int, int> inserted_positions) {
  Delete(deleted_node_id);
  Insert(deleted_node_id, target_node_id, inserted_positions);
}

// Private

BStarTree::Node::Node(int parent_id, int left_child_id, int right_child_id)
    : parent_id_(parent_id),
      left_child_id_(left_child_id),
      right_child_id_(right_child_id),
      is_visited_(false) {}

BStarTree::Node& BStarTree::node(int node_id) { return nodes_.at(node_id); }

const BStarTree::Node& BStarTree::node(int node_id) const {
  return nodes_.at(node_id);
}

void BStarTree::Delete(int deleted_node_id) {
  Node& deleted_node = node(deleted_node_id);

  int child_id = -1;
  if (deleted_node.left_child_id_ != -1 && deleted_node.right_child_id_ != -1) {
    child_id = deleted_node.left_child_id_;
    int current_node_id = child_id;
    while (left_child_id(current_node_id) != -1 &&
           right_child_id(current_node_id) != -1) {
      current_node_id = left_child_id(current_node_id);
    }
    if (right_child_id(current_node_id) != -1) {
      Node& current_node = node(current_node_id);
      current_node.left_child_id_ = current_node.right_child_id_;
      current_node.right_child_id_ = -1;
    }
    while (current_node_id != deleted_node_id) {
      Node& current_node = node(current_node_id);
      Node& current_parent = node(current_node.parent_id_);
      Node& current_parent_right_child = node(current_parent.right_child_id_);
      current_node.right_child_id_ = current_parent.right_child_id_;
      current_parent_right_child.parent_id_ = current_node_id;
      current_node_id = current_node.parent_id_;
    }
  } else if (deleted_node.left_child_id_ != -1) {
    child_id = deleted_node.left_child_id_;
  } else if (deleted_node.right_child_id_ != -1) {
    child_id = deleted_node.right_child_id_;
  }

  if (child_id != -1) {
    node(child_id).parent_id_ = deleted_node.parent_id_;
  }

  if (deleted_node.parent_id_ == -1) {
    root_id_ = child_id;
  } else {
    Node& parent = node(deleted_node.parent_id_);
    if (parent.left_child_id_ == deleted_node_id) {
      parent.left_child_id_ = child_id;
    } else {
      parent.right_child_id_ = child_id;
    }
  }

  deleted_node.parent_id_ = -1;
  deleted_node.left_child_id_ = -1;
  deleted_node.right_child_id_ = -1;
}

void BStarTree::Insert(int inserted_node_id, int target_node_id,
                       pair<int, int> inserted_positions) {
  Node& inserted_node = node(inserted_node_id);
  Node& target_node = node(target_node_id);
  inserted_node.parent_id_ = target_node_id;
  if (inserted_positions.first % 2 == 0) {
    if (inserted_positions.second % 2 == 0) {
      inserted_node.left_child_id_ = target_node.left_child_id_;
    } else {
      inserted_node.right_child_id_ = target_node.left_child_id_;
    }
    if (target_node.left_child_id_ != -1) {
      node(target_node.left_child_id_).parent_id_ = inserted_node_id;
    }
    target_node.left_child_id_ = inserted_node_id;
  } else {
    if (inserted_positions.second % 2 == 0) {
      inserted_node.left_child_id_ = target_node.right_child_id_;
    } else {
      inserted_node.right_child_id_ = target_node.right_child_id_;
    }
    if (target_node.right_child_id_ != -1) {
      node(target_node.right_child_id_).parent_id_ = inserted_node_id;
    }
    target_node.right_child_id_ = inserted_node_id;
  }
}
