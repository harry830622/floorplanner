#include "./macro.hpp"

using namespace std;

Macro::Macro(const string& name, double width, double height, bool is_rotatable)
    : name_(name),
      width_(width),
      height_(height),
      is_rotatable_(is_rotatable) {}

void Macro::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Macro:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "name_: " << name_ << endl;
  os << string(indent_level * indent_size, ' ') << "width_: " << width_ << endl;
  os << string(indent_level * indent_size, ' ') << "height_: " << height_
     << endl;
}

const string& Macro::name() const { return name_; }

double Macro::width() const { return width_; }

double Macro::height() const { return height_; }

bool Macro::is_rotatable() const { return is_rotatable_; }
