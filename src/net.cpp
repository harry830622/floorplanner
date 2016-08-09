#include <climits>
#include "net.hpp"

int Net::HPWL()
{
  int min_x = INT_MAX;
  int min_y = INT_MAX;
  int max_x = 0;
  int max_y = 0;

  for (size_t i = 0; i < terminals_.size(); ++i) {
    int x = terminals_[i]->coordinate_.x_;
    int y = terminals_[i]->coordinate_.y_;

    if (x < min_x) {
      min_x = x;
    }
    if (y < min_y) {
      min_y = y;
    }
    if (x > max_x) {
      max_x = x;
    }
    if (y > max_y) {
      max_y = y;
    }
  }

  return max_x - min_x + max_y - min_y;
}
