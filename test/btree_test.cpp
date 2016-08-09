#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../btree.hpp"
#include "gtest/gtest.h"

class BTreeTest :public testing::Test
{
  protected:
    BTreeTest() :btree_(new vector<Macro*>(1, new Macro("root", 10, 20)))
    {
    }

    virtual ~BTreeTest()
    {
      delete btree_.macros_;
    }

    virtual void SetUp()
    {
      root_ = btree_.root_;
      a_ = new Macro("a", 80, 10);
      b_ = new Macro("b", 40, 90);
      c_ = new Macro("c", 90, 90);
      d_ = new Macro("d", 10, 70);
      e_ = new Macro("e", 10, 60);
      f_ = new Macro("f", 20, 10);
      g_ = new Macro("g", 30, 10);
      h_ = new Macro("h", 10, 10);
      i_ = new Macro("i", 70, 30);
      j_ = new Macro("j", 20, 40);
      k_ = new Macro("k", 30, 20);
      l_ = new Macro("l", 50, 20);

      btree_.macros_->push_back(a_);
      btree_.macros_->push_back(b_);
      btree_.macros_->push_back(c_);
      btree_.macros_->push_back(d_);
      btree_.macros_->push_back(e_);
      btree_.macros_->push_back(f_);
      btree_.macros_->push_back(g_);
      btree_.macros_->push_back(h_);
      btree_.macros_->push_back(i_);
      btree_.macros_->push_back(j_);
      btree_.macros_->push_back(k_);
      btree_.macros_->push_back(l_);

      root_->left_child_ = a_;
      a_->parent_ = root_;
      root_->right_child_ = b_;
      b_->parent_ = root_;

      a_->left_child_ = c_;
      c_->parent_ = a_;
      a_->right_child_ = d_;
      d_->parent_ = a_;

      b_->left_child_ = e_;
      e_->parent_ = b_;
      b_->right_child_ = f_;
      f_->parent_ = b_;

      c_->left_child_ = g_;
      g_->parent_ = c_;
      c_->right_child_ = h_;
      h_->parent_ = c_;

      e_->left_child_ = j_;
      j_->parent_ = e_;

      f_->right_child_ = k_;
      k_->parent_ = f_;

      g_->right_child_ = i_;
      i_->parent_ = g_;

      k_->right_child_ = l_;
      l_->parent_ = k_;

      btree_.PrintMacros();
      cout << endl;
    }

    virtual void TearDown()
    {
      for (size_t i = 0; i < btree_.macros_->size(); ++i) {
        delete btree_.macros_->at(i);
      }
    }

    BTree btree_;
    Macro* root_;
    Macro* a_;
    Macro* b_;
    Macro* c_;
    Macro* d_;
    Macro* e_;
    Macro* f_;
    Macro* g_;
    Macro* h_;
    Macro* i_;
    Macro* j_;
    Macro* k_;
    Macro* l_;
};

TEST_F(BTreeTest, DeleteZeroChildMacroL)
{
  btree_.DeleteMacro(l_);

  EXPECT_EQ(0, l_->parent_);
  EXPECT_EQ(0, l_->left_child_);
  EXPECT_EQ(0, l_->right_child_);

  EXPECT_EQ(0, k_->right_child_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, DeleteOneChildMacroG)
{
  btree_.DeleteMacro(g_);

  EXPECT_EQ(0, g_->parent_);
  EXPECT_EQ(0, g_->left_child_);
  EXPECT_EQ(0, g_->right_child_);

  EXPECT_EQ("i", c_->left_child_->name_);

  EXPECT_EQ("c", i_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, DeleteTwoChildMacroA)
{
  btree_.DeleteMacro(a_);

  EXPECT_EQ(0, a_->parent_);
  EXPECT_EQ(0, a_->left_child_);
  EXPECT_EQ(0, a_->right_child_);

  EXPECT_EQ("c", root_->left_child_->name_);

  EXPECT_EQ("root", c_->parent_->name_);
  EXPECT_EQ("g", c_->left_child_->name_);
  EXPECT_EQ("d", c_->right_child_->name_);

  EXPECT_EQ("c", d_->parent_->name_);

  EXPECT_EQ("c", g_->parent_->name_);
  EXPECT_EQ("i", g_->left_child_->name_);
  EXPECT_EQ("h", g_->right_child_->name_);

  EXPECT_EQ("g", h_->parent_->name_);

  EXPECT_EQ("g", i_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, DeleteTwoChildMacroRoot)
{
  btree_.DeleteMacro(root_);

  EXPECT_EQ(0, root_->parent_);
  EXPECT_EQ(0, root_->left_child_);
  EXPECT_EQ(0, root_->right_child_);

  EXPECT_EQ(0, a_->parent_);
  EXPECT_EQ("c", a_->left_child_->name_);
  EXPECT_EQ("b", a_->right_child_->name_);

  EXPECT_EQ("a", b_->parent_->name_);

  EXPECT_EQ("a", c_->parent_->name_);
  EXPECT_EQ("g", c_->left_child_->name_);
  EXPECT_EQ("d", c_->right_child_->name_);

  EXPECT_EQ("c", d_->parent_->name_);

  EXPECT_EQ("c", g_->parent_->name_);
  EXPECT_EQ("i", g_->left_child_->name_);
  EXPECT_EQ("h", g_->right_child_->name_);

  EXPECT_EQ("g", h_->parent_->name_);

  EXPECT_EQ("g", i_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, InsertMacroLToRootRight)
{
  btree_.DeleteMacro(l_);
  btree_.InsertMacro(l_, root_, 1);

  EXPECT_EQ("l", root_->right_child_->name_);

  EXPECT_EQ("l", b_->parent_->name_);

  EXPECT_EQ("root", l_->parent_->name_);
  EXPECT_EQ(0, l_->left_child_);
  EXPECT_EQ("b", l_->right_child_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, InsertMacroRootToLLeft)
{
  btree_.DeleteMacro(root_);
  btree_.InsertMacro(root_, l_, 0);

  EXPECT_EQ("l", root_->parent_->name_);
  EXPECT_EQ(0, root_->left_child_);
  EXPECT_EQ(0, root_->right_child_);

  EXPECT_EQ("root", l_->left_child_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, SwapMacrosJAndA)
{
  btree_.SwapMacros(j_, a_);

  EXPECT_EQ("j", root_->left_child_->name_);

  EXPECT_EQ("e", a_->parent_->name_);

  EXPECT_EQ("j", c_->parent_->name_);

  EXPECT_EQ("j", d_->parent_->name_);

  EXPECT_EQ("a", e_->left_child_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, SwapMacrosRootAndF)
{
  btree_.SwapMacros(root_, f_);

  EXPECT_EQ("b", root_->parent_->name_);
  EXPECT_EQ(0, root_->left_child_);
  EXPECT_EQ("k", root_->right_child_->name_);

  EXPECT_EQ("f", a_->parent_->name_);

  EXPECT_EQ("f", b_->parent_->name_);

  EXPECT_EQ(0, f_->parent_);
  EXPECT_EQ("a", f_->left_child_->name_);
  EXPECT_EQ("b", f_->right_child_->name_);

  EXPECT_EQ("root", k_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, SwapMacrosAAndRoot)
{
  btree_.SwapMacros(a_, root_);

  EXPECT_EQ("a", root_->parent_->name_);
  EXPECT_EQ("c", root_->left_child_->name_);
  EXPECT_EQ("d", root_->right_child_->name_);

  EXPECT_EQ(0, a_->parent_);
  EXPECT_EQ("root", a_->left_child_->name_);
  EXPECT_EQ("b", a_->right_child_->name_);

  EXPECT_EQ("a", b_->parent_->name_);

  EXPECT_EQ("root", c_->parent_->name_);

  EXPECT_EQ("root", d_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, SwapMacrosFAndK)
{
  btree_.SwapMacros(f_, k_);

  EXPECT_EQ("k", b_->right_child_->name_);

  EXPECT_EQ("k", f_->parent_->name_);
  EXPECT_EQ(0, f_->left_child_);
  EXPECT_EQ("l", f_->right_child_->name_);

  EXPECT_EQ("b", k_->parent_->name_);
  EXPECT_EQ(0, k_->left_child_);
  EXPECT_EQ("f", k_->right_child_->name_);

  EXPECT_EQ("f", l_->parent_->name_);

  btree_.PrintMacros();
}

TEST_F(BTreeTest, Perturb)
{
  for (int i = 0; i < 13; ++i) {
    btree_.macros_->at(i)->is_rotatable_ = true;
    btree_.Perturb();
  }

  btree_.PrintMacros();
}

TEST_F(BTreeTest, RecoverMacros)
{
  for (int i = 0; i < 13; ++i) {
    btree_.macros_->at(i)->is_rotatable_ = true;
    btree_.SnapshotAllMacros();
    btree_.Perturb();
    btree_.RecoverAllMacros();
  }

  btree_.PrintMacros();
}

TEST_F(BTreeTest, PackMacros)
{
  btree_.PackMacros();

  btree_.contour_.Print();
  cout << btree_.bounding_width_ << " " << btree_.bounding_height_ << endl;
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  srand(time(NULL));

  return RUN_ALL_TESTS();
}
