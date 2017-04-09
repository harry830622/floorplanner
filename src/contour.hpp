#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "./point.hpp"

#include <list>

class Contour {
 public:
  Contour();

  double max_x() const;
  double max_y() const;

  double FindMaxYBetween(double x_begin, double x_end) const;

  void Update(double x, double width, double height);

 private:
  double max_y_;
  std::list<Point> coordinates_;
};

#endif
