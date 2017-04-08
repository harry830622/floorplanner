#ifndef B_STAR_TREE_HPP
#define B_STAR_TREE_HPP

#include <vector>

class BStarTree {
 public:
  BStarTree(int num_macros);

  int root_id() const;
  int left_child_id(int node_id) const;
  int right_child_id(int node_id) const;

  void DeleteAndInsert(int source_node_id, int target_node_id,
                       bool is_to_insert_left);

 private:
  class Node {
   public:
    int left_child_id_;
    int right_child_id_;
  };

  void Delete(int node_id);
  void Insert(int node_id, bool is_to_insert_left);

  int root_id_;
  std::vector<Node> nodes_;
};

#endif
