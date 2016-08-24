#include "contour.hpp"

using helpers::Point;

Contour::Contour() : points_(1, Point<int>(0, 0)) {}

int Contour::max_x() const { return points_.back().x(); }

int Contour::MaxYBetween(int x_begin, int x_end) const {
  auto it = points_.begin();

  while (it->x() <= x_begin && it != points_.end()) {
    ++it;
  }
  --it;

  int max_y = 0;
  while (it->x() < x_end && it != points_.end()) {
    if (it->y() > max_y) {
      max_y = it->y();
    }
    ++it;
  }

  return max_y;
}

int Contour::MaxY() const { return MaxYBetween(0, max_x()); }

void Contour::Update(int x, int width, int height) {
  int y = MaxYBetween(x, x + width);

  auto it = points_.begin();
  while (it->x() < x && it != points_.end()) {
    ++it;
  }
  auto it_begin = it;

  bool is_equal = true;
  int last_y = 0;
  while (it->x() < x + width && it != points_.end()) {
    ++it;
  }
  if (it == points_.end() || it->x() != x + width) {
    is_equal = false;
    last_y = (--it)->y();
    ++it;
  }

  points_.erase(it_begin, it);
  points_.insert(it, Point<int>(x, y + height));
  if (!is_equal) {
    points_.insert(it, Point<int>(x + width, last_y));
  }

  last_y = -1;
  for (it = points_.begin(); it != points_.end(); ++it) {
    if (it->y() == last_y) {
      it = points_.erase(it);
      --it;
    } else {
      last_y = it->y();
    }
  }
}
