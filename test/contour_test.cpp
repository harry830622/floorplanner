#include "../src/contour.hpp"

#include <gtest/gtest.h>

class ContourTest : public ::testing::Test {
 protected:
  Contour contour_;
};

TEST_F(ContourTest, Initialization) {
  EXPECT_EQ(0, contour_.max_x());
  EXPECT_EQ(0, contour_.max_y());
}

TEST_F(ContourTest, Update) {
  contour_.Update(0, 1.0, 1.0);
  EXPECT_EQ(1.0, contour_.max_x());
  EXPECT_EQ(1.0, contour_.max_y());
  contour_.Update(1.0, 2.0, 3.0);
  EXPECT_EQ(3.0, contour_.max_x());
  EXPECT_EQ(3.0, contour_.max_y());
  contour_.Update(3.0, 4.0, 5.0);
  EXPECT_EQ(7.0, contour_.max_x());
  EXPECT_EQ(5.0, contour_.max_y());
  contour_.Update(1.0, 7.0, 2.0);
  EXPECT_EQ(8.0, contour_.max_x());
  EXPECT_EQ(7.0, contour_.max_y());
  contour_.Update(8.0, 2.0, 1.0);
  EXPECT_EQ(10.0, contour_.max_x());
  EXPECT_EQ(7.0, contour_.max_y());
  EXPECT_EQ(1.0, contour_.FindMaxYBetween(0.0, 1.0));
  EXPECT_EQ(7.0, contour_.FindMaxYBetween(0.0, 2.0));
  EXPECT_EQ(7.0, contour_.FindMaxYBetween(0.0, 11.0));
  EXPECT_EQ(1.0, contour_.FindMaxYBetween(8.0, 11.0));
}
