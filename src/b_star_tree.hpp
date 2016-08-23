#ifndef B_STAR_TREE_HPP
#define B_STAR_TREE_HPP

#include <vector>

class BStarTree {
 public:
  BStarTree(int num_nodes = 0);

  int num_nodes() const;

  void Print(int indent = 0) const;

  void DeleteThenInsertNode(int node_id, int target_node_id);
  void SwapNodes(int node_a_id, int node_b_id);

 private:
  class Node {
   public:
    Node(int macro_id);

    int macro_id_;
    int parent_id_;
    int left_child_id_;
    int right_child_id_;
    bool is_visited_;
  };

  void Skew();
  void DeleteNode(int node_id);
  void InsertNode(int node_id, int target_node_id);

  std::vector<Node> nodes_;
  int root_id_;
};

#endif
