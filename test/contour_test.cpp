#include "../src/contour.hpp"

#include <gtest/gtest.h>

using namespace std;

class ContourTest : public ::testing::Test {
 protected:
  Contour contour_;
};

TEST_F(ContourTest, Initialization) {
  EXPECT_EQ(0, contour_.max_x());
  EXPECT_EQ(0, contour_.max_y());
}

TEST_F(ContourTest, Update) {
  auto t = contour_.Update(0, 1.0, 1.0);
  /* get<0>(t).Print(); */
  /* get<1>(t).Print(); */
  /* contour_.Print(); */
  EXPECT_EQ(1.0, contour_.max_x());
  EXPECT_EQ(1.0, contour_.max_y());
  t = contour_.Update(1.0, 2.0, 3.0, get<2>(t));
  /* get<0>(t).Print(); */
  /* get<1>(t).Print(); */
  /* contour_.Print(); */
  EXPECT_EQ(3.0, contour_.max_x());
  EXPECT_EQ(3.0, contour_.max_y());
  t = contour_.Update(3.0, 4.0, 5.0, get<2>(t));
  /* get<0>(t).Print(); */
  /* get<1>(t).Print(); */
  /* contour_.Print(); */
  EXPECT_EQ(7.0, contour_.max_x());
  EXPECT_EQ(5.0, contour_.max_y());
  t = contour_.Update(1.0, 7.0, 2.0, get<2>(t));
  /* get<0>(t).Print(); */
  /* get<1>(t).Print(); */
  /* contour_.Print(); */
  EXPECT_EQ(8.0, contour_.max_x());
  EXPECT_EQ(7.0, contour_.max_y());
  t = contour_.Update(8.0, 2.0, 1.0, get<2>(t));
  /* get<0>(t).Print(); */
  /* get<1>(t).Print(); */
  /* contour_.Print(); */
  EXPECT_EQ(10.0, contour_.max_x());
  EXPECT_EQ(7.0, contour_.max_y());
}
