#include <iostream>
#include "contour.hpp"

Contour::Contour()
{
  points_.push_back(Point(0, 0));
}

int Contour::FindMaxY(int x_begin, int x_end)
{
  list<Point>::iterator it = points_.begin();

  while (it->x_ <= x_begin && it != points_.end()) {
    ++it;
  }
  --it;

  int max_y = 0;
  while (it->x_ < x_end && it != points_.end()) {
    if (it->y_ > max_y) {
      max_y = it->y_;
    }
    ++it;
  }

  return max_y;
}

void Contour::Update(Macro* macro_inserted)
{
  list<Point>::iterator it = points_.begin();

  list<Point>::iterator it_begin;
  while (it->x_ < macro_inserted->GetX() && it != points_.end()) {
    ++it;
  }
  it_begin = it;

  while (it->x_ < macro_inserted->GetX() + macro_inserted->width_ && it != points_.end()) {
    ++it;
  }
  int last_y = 0;
  bool is_equal = true;
  if (it == points_.end() || it->x_ != macro_inserted->GetX() + macro_inserted->width_) {
    is_equal = false;
    last_y = (--it)->y_;
    ++it;
  }

  points_.erase(it_begin, it);
  points_.insert(it, Point(macro_inserted->GetX(), macro_inserted->GetY() + macro_inserted->height_));
  if (!is_equal) {
    points_.insert(it, Point(macro_inserted->GetX() + macro_inserted->width_, last_y));
  }

  last_y = -1;
  for (it = points_.begin(); it != points_.end(); ++it) {
    if (it->y_ == last_y) {
      it = points_.erase(it);
      --it;
    } else {
      last_y = it->y_;
    }
  }
}

void Contour::Print()
{
  list<Point>::iterator it = points_.begin();
  while (it != points_.end()) {
    cout << "(" << it->x_ << ", " << it->y_ << ") ";
    ++it;
  }
  cout << endl;
}
