#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class Point {
 public:
  Point(double x, double y);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  double x() const;
  double y() const;

 private:
  double x_;
  double y_;
};

#endif
