#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <string>
#include "point.hpp"

using namespace std;

class Terminal
{
  public:
    Terminal(string name, int x = 0, int y = 0) :name_(name), coordinate_(x, y){}

    string name_;
    Point coordinate_;
};

#endif
