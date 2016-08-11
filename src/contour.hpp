#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <list>

class Contour {
  public:
    Contour() : points_(1, std::make_pair(0, 0)) {}

    int FindMaxY(int x_begin, int x_end) const;

    void Update(int x, int width, int height);

  private:
    std::list<std::pair<int, int>> points_;
};

#endif
