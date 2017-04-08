#include "./point.hpp"

using namespace std;

Point::Point(int x, int y) : x_(x), y_(y) {}

void Point::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Point:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "x_: " << x_ << endl;
  os << string(indent_level * indent_size, ' ') << "y_: " << y_ << endl;
}

int Point::x() const { return x_; }

int Point::y() const { return y_; }
