#ifndef NET_HPP
#define NET_HPP

#include "./point.hpp"

#include <vector>

class Net {
 public:
  Net(const std::vector<int>& macro_ids,
      const std::vector<Point>& terminal_coordinates);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  int num_macros() const;
  int macro_id(int nth_macro) const;

  double ComputeWirelength(
      std::vector<std::pair<Point, Point>> macro_bounding_box_by_id) const;

 private:
  double min_x_;
  double min_y_;
  double max_x_;
  double max_y_;
  std::vector<int> macro_ids_;
};

#endif
