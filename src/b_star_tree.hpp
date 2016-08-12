#ifndef B_STAR_TREE_HPP
#define B_STAR_TREE_HPP

#include <iostream>
#include <vector>

class BStarTree {
 public:
  BStarTree() : root_idx_(-1) {}

  int GetRootNodeIdx() const;

  int GetNumNodes() const;
  int GetNodeMacroIdx(int idx) const;
  int GetNodeParentIdx(int idx) const;
  int GetNodeLeftChildIdx(int idx) const;
  int GetNodeRightChildIdx(int idx) const;
  int GetNodeX(int idx) const;
  int GetNodeY(int idx) const;
  bool GetNodeIsRotated(int idx) const;

  int CalculateHeight() const;
  int CalculateChipWidth() const;
  int CalculateChipHeight() const;
  BStarTree Perturb() const;

  void SetNodeX(int idx, int x);
  void SetNodeY(int idx, int y);

  int AddNewNode(int macro_idx);

  void Skew();
  void UpdateContour(int x, int width, int height);

 private:
  class Node {
   public:
    Node(int macro_idx)
        : macro_idx_(macro_idx),
          parent_idx_(-1),
          left_child_idx_(-1),
          right_child_idx_(-1),
          x_(0),
          y_(0),
          is_rotated_(false) {}

    int macro_idx_;

    int parent_idx_;
    int left_child_idx_;
    int right_child_idx_;

    int x_, y_;
    bool is_rotated_;
  };

  int Height(int root_idx) const;

  void RotateNode(int idx);
  void SwapNodes(int idx_a, int idx_b);
  void DeleteNode(int idx);
  void InsertNode(int idx, int target_node_idx, bool to_left);

  int root_idx_;
  std::vector<Node> nodes_;
};

std::ostream& operator<<(std::ostream& os, const BStarTree& b_star_tree);

#endif
