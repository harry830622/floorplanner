#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class Point {
 public:
  Point(int x, int y);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  int x() const;
  int y() const;

 private:
  int x_;
  int y_;
};

#endif
