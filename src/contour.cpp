#include "./contour.hpp"

Contour::Contour() : max_y_(0.0), coordinates_(1, Point(0.0, 0.0)) {}

double Contour::max_x() const { return coordinates_.back().x(); }

double Contour::max_y() const { return max_y_; }

double Contour::FindMaxYBetween(double x_begin, double x_end) const {
  auto it = coordinates_.begin();

  while (it != coordinates_.end() && it->x() <= x_begin) {
    ++it;
  }
  --it;

  double max_y = 0;
  while (it != coordinates_.end() && it->x() < x_end) {
    if (it->y() > max_y) {
      max_y = it->y();
    }
    ++it;
  }

  return max_y;
}

void Contour::Update(double x, double width, double height) {
  const double new_y = FindMaxYBetween(x, x + width) + height;
  if (new_y > max_y_) {
    max_y_ = new_y;
  }

  auto it = coordinates_.begin();
  while (it->x() < x) {
    ++it;
  }
  const auto it_begin = it;
  while (it != coordinates_.end() && it->x() < x + width) {
    ++it;
  }
  const auto it_end = it;

  const bool is_equal = [&]() {
    if (it_end == coordinates_.end() || it_end->x() != x + width) {
      return false;
    }
    return true;
  }();
  const double last_y = prev(it_end)->y();

  coordinates_.erase(it_begin, it_end);
  coordinates_.insert(it_end, Point(x, new_y));
  if (!is_equal) {
    coordinates_.insert(it_end, Point(x + width, last_y));
  }

  if (it_end->y() == prev(it_end)->y()) {
    coordinates_.erase(it_end);
  }
}
