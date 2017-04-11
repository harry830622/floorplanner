#include "./terminal.hpp"

using namespace std;

Terminal::Terminal(const string& name, double x, double y)
    : name_(name), coordinates_(x, y) {}

void Terminal::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Terminal:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "name_: " << name_ << endl;
  os << string(indent_level * indent_size, ' ') << "coordinates_:" << endl;
  coordinates_.Print(os, indent_level + 1);
}

const string& Terminal::name() const { return name_; }

const Point& Terminal::coordinates() const { return coordinates_; }

double Terminal::x() const { return coordinates_.x(); }

double Terminal::y() const { return coordinates_.y(); }
