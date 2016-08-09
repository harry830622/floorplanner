#include "../contour.hpp"
#include "gtest/gtest.h"

class ContourTest :public testing::Test
{
  protected:
    Contour contour;
    virtual void SetUp()
    {
    }
};

TEST_F(ContourTest, Contructor)
{
  EXPECT_EQ(1, contour.points_.size());
}

TEST_F(ContourTest, FindMaxY)
{
  contour.points_.push_back(Point(2, 4));
  contour.points_.push_back(Point(5, 1));
  contour.points_.push_back(Point(6, 8));
  contour.points_.push_back(Point(9, 0));
  EXPECT_EQ(0, contour.FindMaxY(0, 0));
  EXPECT_EQ(0, contour.FindMaxY(0, 1));
  EXPECT_EQ(0, contour.FindMaxY(0, 2));
  EXPECT_EQ(0, contour.FindMaxY(9, 100));
  EXPECT_EQ(4, contour.FindMaxY(2, 5));
  EXPECT_EQ(4, contour.FindMaxY(2, 6));
  EXPECT_EQ(4, contour.FindMaxY(3, 5));
  EXPECT_EQ(8, contour.FindMaxY(1, 8));
  EXPECT_EQ(8, contour.FindMaxY(0, 100));
}

TEST_F(ContourTest, Update)
{
  Macro ma("a", 10, 8);
  contour.Update(&ma);
  contour.Print();

  Macro mb("b", 7, 3);
  mb.bottom_left_.x_ = 10;
  mb.bottom_left_.y_ = 0;
  contour.Update(&mb);
  contour.Print();

  Macro mc("c", 4, 4);
  mc.bottom_left_.x_ = 10;
  mc.bottom_left_.y_ = 3;
  contour.Update(&mc);
  contour.Print();

  Macro md("d", 5, 10);
  md.bottom_left_.x_ = 10;
  md.bottom_left_.y_ = 7;
  contour.Update(&md);
  contour.Print();

  Macro me("e", 2, 4);
  me.bottom_left_.x_ = 0;
  me.bottom_left_.y_ = 8;
  contour.Update(&me);
  contour.Print();

  Macro mf("f", 3, 2);
  mf.bottom_left_.x_ = 2;
  mf.bottom_left_.y_ = 8;
  contour.Update(&mf);
  contour.Print();

  Macro mg("g", 2, 5);
  mg.bottom_left_.x_ = 5;
  mg.bottom_left_.y_ = 8;
  contour.Update(&mg);
  contour.Print();

  Macro mh("h", 2, 3);
  mh.bottom_left_.x_ = 7;
  mh.bottom_left_.y_ = 8;
  contour.Update(&mh);
  contour.Print();

  Macro mi("i", 4, 4);
  mi.bottom_left_.x_ = 0;
  mi.bottom_left_.y_ = 12;
  contour.Update(&mi);
  contour.Print();

  Macro mj("j", 4, 3);
  mj.bottom_left_.x_ = 4;
  mj.bottom_left_.y_ = 13;
  contour.Update(&mj);
  contour.Print();
}
