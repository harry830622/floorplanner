#include "contour.hpp"

using namespace std;

int Contour::FindMaxX() const { return points_.back().first; }

int Contour::FindMaxY() const {
  return FindMaxYBetween(0, points_.back().first);
}

void Contour::Reset() {
  points_.clear();
  points_.push_back(make_pair(0, 0));
}

void Contour::Update(int x, int width, int height) {
  auto it = points_.begin();

  while (it->first < x && it != points_.end()) {
    ++it;
  }
  auto it_begin = it;

  int last_y = 0;
  bool is_equal = true;
  while (it->first < x + width && it != points_.end()) {
    ++it;
  }
  if (it == points_.end() || it->first != x + width) {
    is_equal = false;
    last_y = (--it)->second;
    ++it;
  }

  points_.erase(it_begin, it);
  points_.insert(it, make_pair(x, FindMaxYBetween(x, x + width) + height));
  if (!is_equal) {
    points_.insert(it, make_pair(x + width, last_y));
  }

  last_y = -1;
  for (it = points_.begin(); it != points_.end(); ++it) {
    if (it->second == last_y) {
      it = points_.erase(it);
      --it;
    } else {
      last_y = it->second;
    }
  }
}

int Contour::FindMaxYBetween(int x_begin, int x_end) const {
  auto it = points_.begin();

  while (it->first <= x_begin && it != points_.end()) {
    ++it;
  }
  --it;

  int max_y = 0;
  while (it->first < x_end && it != points_.end()) {
    if (it->second > max_y) {
      max_y = it->second;
    }
    ++it;
  }

  return max_y;
}
