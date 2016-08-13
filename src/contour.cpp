#include "contour.hpp"

using namespace std;

int Contour::GetNumPoints() const { return points_.size(); }

int Contour::GetPointX(int idx) const {
  auto it = points_.begin();
  for (int i = 0; i < idx; ++i) {
    ++it;
  }
  return it->first;
}

int Contour::GetPointY(int idx) const {
  auto it = points_.begin();
  for (int i = 0; i < idx; ++i) {
    ++it;
  }
  return it->second;
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

int Contour::FindMaxX() const { return points_.back().first; }

int Contour::FindMaxY() const {
  return FindMaxYBetween(0, points_.back().first);
}

void Contour::Reset() {
  points_.clear();
  points_.push_back(make_pair(0, 0));
}

void Contour::Update(int x, int width, int height) {
  int y = FindMaxYBetween(x, x + width);

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
  points_.insert(it, make_pair(x, y + height));
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

ostream& operator<<(ostream& os, const Contour& contour) {
  const int indent = 2;
  os << "Contour:" << endl;
  os << string(indent, ' ') << "points_: ";
  for (int i = 0; i < contour.GetNumPoints(); ++i) {
    os << "(" << contour.GetPointX(i) << ", " << contour.GetPointY(i) << ") ";
  }
  os << endl;

  return os;
}
