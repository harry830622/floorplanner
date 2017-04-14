#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "./point.hpp"

#include <list>

class Contour {
 public:
  Contour();

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  double max_x() const;
  double max_y() const;

  std::pair<Point, Point> Update(double x, double width, double height);

 private:
  double FindMaxYBetween(double x_begin, double x_end) const;

  double max_y_;
  std::list<Point> coordinates_;
};

#endif
