#include "../src/b_star_tree.hpp"

#include <gtest/gtest.h>

class BStarTreeTest : public ::testing::Test {
 protected:
  static const int num_nodes = 5;
  BStarTreeTest() : b_star_tree_(num_nodes) {}
  BStarTree b_star_tree_;
};

TEST_F(BStarTreeTest, Initialization) {
  const int num_nodes = BStarTreeTest::num_nodes;
  const int root_id = b_star_tree_.root_id();
  EXPECT_EQ(0, root_id);
  int nth_node = 0;
  int current_node_id = root_id;
  while (current_node_id != -1) {
    ++nth_node;
    const int parent_id = b_star_tree_.parent_id(current_node_id);
    const int left_child_id = b_star_tree_.left_child_id(current_node_id);
    const int right_child_id = b_star_tree_.right_child_id(current_node_id);
    EXPECT_EQ(current_node_id - 1, parent_id);
    if (nth_node != num_nodes) {
      EXPECT_EQ(current_node_id + 1, left_child_id);
    } else {
      EXPECT_EQ(-1, left_child_id);
    }
    EXPECT_EQ(-1, right_child_id);
    current_node_id = left_child_id;
  }
  EXPECT_EQ(num_nodes, nth_node);
}

TEST_F(BStarTreeTest, DeletionAndInsertion) {
  const int num_nodes = BStarTreeTest::num_nodes;
  [&]() {
    const int deleted_node_id = num_nodes - 1;
    const int target_node_id =
        b_star_tree_.parent_id(b_star_tree_.parent_id(deleted_node_id));
    b_star_tree_.DeleteAndInsert(deleted_node_id, target_node_id, false);
    EXPECT_EQ(target_node_id, b_star_tree_.parent_id(deleted_node_id));
    EXPECT_EQ(-1, b_star_tree_.left_child_id(deleted_node_id));
    EXPECT_EQ(-1, b_star_tree_.right_child_id(deleted_node_id));
    EXPECT_EQ(deleted_node_id, b_star_tree_.right_child_id(target_node_id));
  }();
  [&]() {
    const int deleted_node_id = num_nodes - 3;
    const int deleted_node_parent_id = b_star_tree_.parent_id(deleted_node_id);
    const int deleted_node_left_id =
        b_star_tree_.left_child_id(deleted_node_id);
    const int deleted_node_right_id =
        b_star_tree_.right_child_id(deleted_node_id);
    const int target_node_id = b_star_tree_.root_id();
    b_star_tree_.DeleteAndInsert(deleted_node_id, target_node_id, false);
    EXPECT_EQ(deleted_node_parent_id,
              b_star_tree_.parent_id(deleted_node_left_id));
    EXPECT_EQ(-1, b_star_tree_.left_child_id(deleted_node_left_id));
    EXPECT_EQ(deleted_node_right_id,
              b_star_tree_.right_child_id(deleted_node_left_id));
    EXPECT_EQ(deleted_node_left_id,
              b_star_tree_.parent_id(deleted_node_right_id));
    EXPECT_EQ(-1, b_star_tree_.left_child_id(deleted_node_right_id));
    EXPECT_EQ(-1, b_star_tree_.right_child_id(deleted_node_right_id));
    EXPECT_EQ(target_node_id, b_star_tree_.parent_id(deleted_node_id));
    EXPECT_EQ(-1, b_star_tree_.left_child_id(deleted_node_id));
    EXPECT_EQ(-1, b_star_tree_.right_child_id(deleted_node_id));
    EXPECT_EQ(deleted_node_id, b_star_tree_.right_child_id(target_node_id));
  }();
}
