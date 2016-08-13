#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <iostream>
#include <list>

class Contour {
 public:
  Contour() : points_(1, std::make_pair(0, 0)) {}
  Contour(const Contour& contour) = default;
  Contour& operator=(const Contour& contour) = default;

  int GetNumPoints() const;
  int GetPointX(int idx) const;
  int GetPointY(int idx) const;

  int FindMaxYBetween(int x_begin, int x_end) const;
  int FindMaxX() const;
  int FindMaxY() const;

  void Reset();
  void Update(int x, int width, int height);

 private:
  std::list<std::pair<int, int>> points_;
};

std::ostream& operator<<(std::ostream& os, const Contour& contour);

#endif
