#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <list>
#include "point.hpp"
#include "macro.hpp"

using namespace std;

class Contour
{
public:
  Contour();

  list<Point> points_;

  int FindMaxY(int x_begin, int x_end);
  void Update(Macro* macro_inserted);
  void Print();
};

#endif
