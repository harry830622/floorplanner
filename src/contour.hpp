#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "./point.hpp"

#include <list>
#include <tuple>

class Contour {
 public:
  Contour();

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  double max_x() const;
  double max_y() const;

  std::tuple<Point, Point, std::list<Point>::iterator> Update(double x,
                                                              double width,
                                                              double height);
  std::tuple<Point, Point, std::list<Point>::iterator> Update(
      double x, double width, double height,
      std::list<Point>::iterator it_hint);

 private:
  static std::list<Point> invalid_list_;
  double FindMaxYBetween(double x_begin, double x_end,
                         std::list<Point>::iterator it_hint) const;

  double max_y_;
  std::list<Point> coordinates_;
};

#endif
