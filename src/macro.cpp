#include "macro.hpp"

Macro::Macro(string name, int width, int height, Terminal* terminal, bool is_rotatable)
  :name_(name), width_(width), height_(height), terminal_(terminal), is_rotatable_(is_rotatable), is_visited_(false), parent_(0), left_child_(0), right_child_(0), old_width_(width), old_height_(height), old_parent_(0), old_left_child_(0), old_right_child_(0), best_width_(width), best_height_(height), best_parent_(0), best_left_child_(0), best_right_child_(0)
{
}

int Macro::GetX()
{
  return bottom_left_.x_;
}

int Macro::GetY()
{
  return bottom_left_.y_;
}

Point Macro::GetCenterPoint()
{
  return Point(GetX() + width_ / 2, GetY() + height_ / 2);
}

void Macro::SetX(int x)
{
  bottom_left_.x_ = x;
}

void Macro::SetY(int y)
{
  bottom_left_.y_ = y;
}

void Macro::Rotate()
{
  if (!is_rotatable_) {
    return;
  }

  int tmp = width_;
  width_ = height_;
  height_ = tmp;
}

void Macro::Snapshot()
{
  old_bottom_left_ = bottom_left_;
  old_width_ = width_;
  old_height_ = height_;
  old_parent_ = parent_;
  old_left_child_ = left_child_;
  old_right_child_ = right_child_;
}

void Macro::SnapshotBest()
{
  best_bottom_left_ = bottom_left_;
  best_width_ = width_;
  best_height_ = height_;
  best_parent_ = parent_;
  best_left_child_ = left_child_;
  best_right_child_ = right_child_;
}

void Macro::RecoverSnapshot()
{
  bottom_left_ = old_bottom_left_;
  terminal_->coordinate_ = old_bottom_left_;
  width_ = old_width_;
  height_ = old_height_;
  parent_ = old_parent_;
  left_child_ = old_left_child_;
  right_child_ = old_right_child_;
}

void Macro::RecoverSnapshotBest()
{
  bottom_left_ = best_bottom_left_;
  terminal_->coordinate_ = best_bottom_left_;
  width_ = best_width_;
  height_ = best_height_;
  parent_ = best_parent_;
  left_child_ = best_left_child_;
  right_child_ = best_right_child_;
  return;
}
