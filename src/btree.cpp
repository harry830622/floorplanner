#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cassert>
#include <queue>
#include <stack>
#include "btree.hpp"

BTree::BTree(vector<Macro*>* macros, vector<Net*>* nets, Macro* root) :macros_(macros), nets_(nets), root_(root), bounding_width_(0), bounding_height_(0), best_bounding_width_(INT_MAX), best_bounding_height_(INT_MAX)
{
  assert(macros_ != 0);
  assert(macros_->size() != 0);

  if (root_ == 0) {
    root_ = macros_->at(0);
  }

  // Initilize B*-tree as a complete binary tree.
  queue<Macro*> q;
  q.push(root_);

  for (size_t i = 1; i < macros_->size(); ++i) {
    Macro* tmp_macro = q.front();
    Macro* tmp_child = macros_->at(i);
    if (tmp_macro->left_child_ == 0) {
      tmp_macro->left_child_ = tmp_child;
    } else {
      tmp_macro->right_child_ = tmp_child;
      q.pop();
    }
    tmp_child->parent_ = tmp_macro;
    q.push(tmp_child);
  }

  PackMacros();
}

void BTree::PrintMacros()
{
  for (size_t i = 0; i < macros_->size(); ++i) {
    Macro* tmp_macro = macros_->at(i);
    cout << tmp_macro->name_ << " x:" << tmp_macro->GetX() << " y:" << tmp_macro->GetY() << " w:" << tmp_macro->width_ << " h:" << tmp_macro->height_ << " ";
    if (tmp_macro->parent_ != 0) {
      cout << "parent_:" << tmp_macro->parent_->name_ << " ";
    }
    if (tmp_macro->left_child_ != 0) {
      cout << "left_child_:" << tmp_macro->left_child_->name_ << " ";
    }
    if (tmp_macro->right_child_ != 0) {
      cout << "right_child_:" << tmp_macro->right_child_->name_ << " ";
    }
    cout << endl;
  }
}

void BTree::DeleteMacro(Macro* macro)
{
  Macro* child = 0;
  if (macro->left_child_ == 0 && macro->right_child_ == 0) {
    //Nothing to do, pass.
  } else if (macro->left_child_ != 0 && macro->right_child_ == 0) {
    child = macro->left_child_;
  } else if (macro->left_child_ == 0 && macro->right_child_ != 0) {
    child = macro->right_child_;
  } else {
    child = macro->left_child_;

    stack<Macro*> s;
    Macro* tmp_macro = child;
    while (tmp_macro->left_child_ != 0 && tmp_macro->right_child_ != 0) {
      s.push(tmp_macro);
      tmp_macro = tmp_macro->left_child_;
    }
    s.push(tmp_macro);

    if (tmp_macro->right_child_ != 0) {
      tmp_macro->left_child_ = tmp_macro->right_child_;
      tmp_macro->right_child_ = 0;
    }

    while (!s.empty()) {
      tmp_macro = s.top();
      s.pop();

      tmp_macro->right_child_ = tmp_macro->parent_->right_child_;
      tmp_macro->parent_->right_child_->parent_ = tmp_macro;
    }
  }

  if (child != 0) {
    child->parent_ = macro->parent_;
  }

  if (macro->parent_ != 0) {
    if (macro->parent_->left_child_ == macro) {
      macro->parent_->left_child_ = child;
    } else {
      macro->parent_->right_child_ = child;
    }
  }

  macro->left_child_ = 0;
  macro->right_child_ = 0;
  macro->parent_ = 0;
}

void BTree::InsertMacro(Macro* macro_to_be_inserted, Macro* macro, int position)
{
  assert(macro_to_be_inserted != 0 && macro != 0);
  assert(position == 0 || position == 1);

  macro_to_be_inserted->parent_ = macro;

  if (position == 0) {
    macro_to_be_inserted->left_child_ = macro->left_child_;
    if (macro_to_be_inserted->left_child_ != 0) {
      macro_to_be_inserted->left_child_->parent_ = macro_to_be_inserted;
    }
    macro->left_child_ = macro_to_be_inserted;
  } else {
    macro_to_be_inserted->right_child_ = macro->right_child_;
    if (macro_to_be_inserted->right_child_ != 0) {
      macro_to_be_inserted->right_child_->parent_ = macro_to_be_inserted;
    }
    macro->right_child_ = macro_to_be_inserted;
  }
}

void BTree::SwapMacros(Macro* macro_a, Macro* macro_b)
{
  assert(macro_a != 0 && macro_b != 0);

  if (macro_a->parent_ == macro_b) {
    Macro* tmp_macro = macro_a;
    macro_a = macro_b;
    macro_b = tmp_macro;
  }

  Macro* tmp_parent = macro_a->parent_;
  Macro* tmp_left_child = macro_a->left_child_;
  Macro* tmp_right_child = macro_a->right_child_;

  if (macro_a->left_child_ == macro_b) {
    tmp_left_child = macro_a;
  } else if (macro_a->right_child_ == macro_b) {
    tmp_right_child = macro_a;
  }

  macro_a->parent_ = macro_b->parent_;
  if (macro_a == macro_a->parent_) {
    macro_a->parent_ = macro_b;
  }
  if (macro_b->parent_ != 0) {
    if (macro_b->parent_->left_child_ == macro_b) {
      macro_b->parent_->left_child_ = macro_a;
    } else {
      macro_b->parent_->right_child_ = macro_a;
    }
  }
  macro_a->left_child_ = macro_b->left_child_;
  if (macro_a->left_child_ != 0) {
    macro_a->left_child_->parent_ = macro_a;
  }
  macro_a->right_child_ = macro_b->right_child_;
  if (macro_a->right_child_ != 0) {
    macro_a->right_child_->parent_ = macro_a;
  }

  macro_b->parent_ = tmp_parent;
  if (tmp_parent != 0) {
    if (tmp_parent->left_child_ == macro_a) {
      macro_b->parent_->left_child_ = macro_b;
    } else {
      macro_b->parent_->right_child_ = macro_b;
    }
  }
  macro_b->left_child_ = tmp_left_child;
  if (macro_b->left_child_ != 0) {
    macro_b->left_child_->parent_ = macro_b;
  }
  macro_b->right_child_ = tmp_right_child;
  if (macro_b->right_child_ != 0) {
    macro_b->right_child_->parent_ = macro_b;
  }
}

void BTree::Perturb()
{
  int op = rand() % 3;
  Macro* macro_a = macros_->at(rand() % macros_->size());
  Macro* macro_b = macros_->at(rand() % macros_->size());
  int position = rand() % 2;

  if (op == 0) {
    while (!macro_a->is_rotatable_) {
      macro_a = macros_->at(rand() % macros_->size());
    }

    macro_a->Rotate();
  } else if (op == 1) {
    while (macro_a == macro_b) {
      macro_a = macros_->at(rand() % macros_->size());
    }

    DeleteMacro(macro_a);
    InsertMacro(macro_a, macro_b, position);
  } else {
    while (macro_a == macro_b) {
      macro_a = macros_->at(rand() % macros_->size());
    }

    SwapMacros(macro_a, macro_b);
  }
}

void BTree::SnapshotAllMacros()
{
  for (size_t i = 0; i < macros_->size(); ++i) {
    macros_->at(i)->Snapshot();
  }
}

void BTree::SnapshotAllMacrosBest()
{
  for (size_t i = 0; i < macros_->size(); ++i) {
    macros_->at(i)->SnapshotBest();
  }
}

void BTree::RecoverAllMacros()
{
  for (size_t i = 0; i < macros_->size(); ++i) {
    macros_->at(i)->RecoverSnapshot();
  }
}

void BTree::RecoverAllMacrosBest()
{
  for (size_t i = 0; i < macros_->size(); ++i) {
    macros_->at(i)->RecoverSnapshotBest();
  }
}

void BTree::SnapshotBestFloorplan()
{
  SnapshotAllMacrosBest();
  best_bounding_width_ = bounding_width_;
  best_bounding_height_ = bounding_height_;
}

void BTree::RecoverBestFloorplan()
{
  RecoverAllMacrosBest();
  bounding_width_ = best_bounding_width_;
  bounding_height_ = best_bounding_height_;
}

void BTree::PackMacros()
{
  contour_.points_.clear();
  contour_.points_.push_back(Point(0, 0));

  for (size_t i = 0; i < macros_->size(); ++i) {
    if (macros_->at(i)->parent_ == 0) {
      root_ = macros_->at(i);
      root_->SetX(0);
      root_->SetY(0);
      root_->terminal_->coordinate_ = root_->GetCenterPoint();
      break;
    }
  }

  stack<Macro*> s;
  s.push(root_);
  contour_.Update(root_);
  bounding_width_ = root_->width_;
  bounding_height_ = root_->height_;
  while (!s.empty()) {
    Macro* tmp_macro = s.top();

    if (tmp_macro->left_child_ != 0 && !tmp_macro->left_child_->is_visited_) {
      Macro* tmp_left_child = tmp_macro->left_child_;
      tmp_left_child->SetX(tmp_macro->GetX() + tmp_macro->width_);
      tmp_left_child->SetY(contour_.FindMaxY(tmp_left_child->GetX(), tmp_left_child->GetX() + tmp_left_child->width_));
      tmp_left_child->terminal_->coordinate_ = tmp_left_child->GetCenterPoint();
      contour_.Update(tmp_left_child);

      if (tmp_left_child->GetX() + tmp_left_child->width_ > bounding_width_) {
        bounding_width_ = tmp_left_child->GetX() + tmp_left_child->width_;
      }
      if (tmp_left_child->GetY() + tmp_left_child->height_ > bounding_height_) {
        bounding_height_ = tmp_left_child->GetY() + tmp_left_child->height_;
      }

      s.push(tmp_macro->left_child_);
    } else if (tmp_macro->right_child_ != 0 && !tmp_macro->right_child_->is_visited_) {
      Macro* tmp_right_child = tmp_macro->right_child_;
      tmp_right_child->SetX(tmp_macro->GetX());
      tmp_right_child->SetY(contour_.FindMaxY(tmp_right_child->GetX(), tmp_right_child->GetX() + tmp_right_child->width_));
      tmp_right_child->terminal_->coordinate_ = tmp_right_child->GetCenterPoint();
      contour_.Update(tmp_right_child);

      if (tmp_right_child->GetX() + tmp_right_child->width_ > bounding_width_) {
        bounding_width_ = tmp_right_child->GetX() + tmp_right_child->width_;
      }
      if (tmp_right_child->GetY() + tmp_right_child->height_ > bounding_height_) {
        bounding_height_ = tmp_right_child->GetY() + tmp_right_child->height_;
      }

      s.push(tmp_macro->right_child_);
    } else {
      tmp_macro->is_visited_ = true;
      s.pop();
    }
  }

  for (size_t i = 0; i < macros_->size(); ++i) {
    macros_->at(i)->is_visited_ = false;
  }
}

int BTree::CalculateArea()
{
  int area = bounding_width_ * bounding_height_;
  assert(area > 0);

  return area;
}

int BTree::CalculateWireLength()
{
  int wire_length = 0;
  for (size_t i = 0; i < nets_->size(); ++i) {
    wire_length += nets_->at(i)->HPWL();
  }
  assert(wire_length > 0);

  return wire_length;
}

double BTree::CalculateCost(double alpha, double outline_ratio, double beta)
{
  double ratio = bounding_height_ / (double)bounding_width_;
  int area = CalculateArea();
  int wire_length = CalculateWireLength();

  return alpha * (double)area + beta * wire_length + (1 - alpha - beta) * (ratio - outline_ratio) * (ratio - outline_ratio) * 10000000;
}

void BTree::SA(double alpha, int outline_width, int outline_height)
{
  const double initial_temperature = 1000000.0;
  const double r = 0.85;
  const size_t num_perturbations = macros_->size() * 100;

  PackMacros();
  double outline_ratio = outline_height / (double)outline_width;
  double old_cost = CalculateCost(alpha, outline_ratio);
  double temperature = initial_temperature;
  while (temperature > 1) {
    for (size_t i = 0; i < num_perturbations; ++i) {
      SnapshotAllMacros();
      Perturb();
      PackMacros();

      double cost = CalculateCost(alpha, outline_ratio);
      double cost_difference = cost - old_cost;
      if (cost_difference > 0 && rand() / (double)RAND_MAX > exp(-1 * cost_difference / temperature)) {
        RecoverAllMacros();
        continue;
      }

      old_cost = cost;

      if (bounding_width_ <= outline_width && bounding_height_ <= outline_height && bounding_width_ < best_bounding_width_ && bounding_height_ < best_bounding_height_) {
        SnapshotBestFloorplan();
      }
    }

    temperature *= r;
  }
}

void BTree::FSA(double alpha, int outline_width, int outline_height)
{
  const size_t num_initial_iterations = macros_->size();
  double sum_area = 0.0;
  double sum_wire_length = 0.0;
  for (size_t i = 0; i < num_initial_iterations; ++i) {
    Perturb();
    PackMacros();

    sum_area += CalculateArea();
    sum_wire_length += CalculateWireLength();
  }

  const double initial_temperature = 1000000.0;
  const double average_area = sum_area / num_initial_iterations;
  const double average_wire_length = sum_wire_length / num_initial_iterations;
  const int k = 7;
  const int c = 100;
  const size_t num_perturbations = macros_->size() * 100;

  double outline_ratio = outline_height / (double)outline_width;
  double old_cost = CalculateCost(alpha, outline_ratio);
  double temperature = initial_temperature;
  int current_iteration = 1;
  while (temperature > 1 && current_iteration < 10) {
    double sum_cost = alpha * CalculateArea() / average_area + (1 - alpha) * CalculateWireLength() / average_wire_length;
    for (size_t i = 0; i < num_perturbations; ++i) {
      SnapshotAllMacros();
      Perturb();
      PackMacros();

      double cost = CalculateCost(alpha, outline_ratio);
      sum_cost += alpha * CalculateArea() / average_area + (1 - alpha) * CalculateWireLength() / average_wire_length;
      double cost_difference = cost - old_cost;
      if (cost_difference > 0 && rand() / (double)RAND_MAX > exp(-1 * cost_difference / temperature)) {
        RecoverAllMacros();
        continue;
      }

      old_cost = cost;

      if (bounding_width_ <= outline_width && bounding_height_ <= outline_height && bounding_width_ < best_bounding_width_ && bounding_height_ < best_bounding_height_) {
        SnapshotBestFloorplan();
      }
    }
    ++current_iteration;

    if (current_iteration <= k) {
      temperature = initial_temperature * sum_cost / num_perturbations / num_perturbations / c;
    } else {
      temperature = initial_temperature * sum_cost / num_perturbations / num_perturbations;
    }
  }
}
