#ifndef B_STAR_TREE_HPP
#define B_STAR_TREE_HPP

#include <vector>

class BStarTree {
 public:
  BStarTree(int num_macros);

  int root_id() const;
  int parent_id(int node_id) const;
  int left_child_id(int node_id) const;
  int right_child_id(int node_id) const;

  void DeleteAndInsert(int deleted_node_id, int target_node_id,
                       bool is_inserted_left);

 private:
  class Node {
   public:
    Node(int parent_id, int left_child_id, int right_child_id);
    int parent_id_;
    int left_child_id_;
    int right_child_id_;
  };

  Node& node(int node_id);
  void Delete(int deleted_node_id);
  void Insert(int inserted_node_id, int target_node_id, bool is_inserted_left);

  int root_id_;
  std::vector<Node> nodes_;
};

#endif
