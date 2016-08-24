#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include "../lib/helpers/helpers.hpp"

#include <list>

class Contour {
  public:
    Contour();

    int max_x() const;

    int MaxYBetween(int x_begin, int x_end) const;
    int MaxY() const;

    void Update(int x, int width, int height);

  private:
    std::list<helpers::Point<int>> points_;
};

#endif
