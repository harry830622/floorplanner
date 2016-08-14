#ifndef B_STAR_TREE_HPP
#define B_STAR_TREE_HPP

#include <iostream>
#include <vector>

class BStarTree {
 public:
  BStarTree() : root_idx_(-1) {}
  BStarTree(const BStarTree& b_star_tree) = default;
  BStarTree& operator=(const BStarTree& b_star_tree) = default;

  int GetRootNodeIdx() const;

  int GetNumNodes() const;
  int GetNodeMacroInstanceIdx(int idx) const;
  int GetNodeParentIdx(int idx) const;
  int GetNodeLeftChildIdx(int idx) const;
  int GetNodeRightChildIdx(int idx) const;
  int GetNodeIsVisited(int idx) const;

  int CalculateHeight() const;

  void SetNodeIsVisited(int idx, bool is_visited);

  int AddNewNode(int macro_instance_idx);

  void Skew();
  void DeleteNode(int idx);
  void InsertNode(int idx, int target_node_idx, char position);
  void SwapNodes(int idx_a, int idx_b);

 private:
  class Node {
   public:
    Node(int macro_instance_idx)
        : macro_instance_idx_(macro_instance_idx),
          parent_idx_(-1),
          left_child_idx_(-1),
          right_child_idx_(-1),
          is_visited_(false) {}
    Node(const Node& node) = default;
    Node& operator=(const Node& node) = default;

    int macro_instance_idx_;
    int parent_idx_;
    int left_child_idx_;
    int right_child_idx_;
    bool is_visited_;
  };

  int Height(int root_idx) const;

  int root_idx_;
  std::vector<Node> nodes_;
};

std::ostream& operator<<(std::ostream& os, const BStarTree& b_star_tree);

#endif
