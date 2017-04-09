#include "./net.hpp"

#include <limits>

using namespace std;

Net::Net(const vector<int>& macro_ids,
         const vector<Point>& terminal_coordinates)
    : min_x_(numeric_limits<double>::max()),
      min_y_(numeric_limits<double>::max()),
      max_x_(numeric_limits<double>::lowest()),
      max_y_(numeric_limits<double>::lowest()),
      macro_ids_(macro_ids) {
  for (const Point& terminal_coordinate : terminal_coordinates) {
    const double terminal_x = terminal_coordinate.x();
    const double terminal_y = terminal_coordinate.y();
    if (terminal_x < min_x_) {
      min_x_ = terminal_x;
    }
    if (terminal_y < min_y_) {
      min_y_ = terminal_y;
    }
    if (terminal_x > max_x_) {
      max_x_ = terminal_x;
    }
    if (terminal_y > max_y_) {
      max_y_ = terminal_y;
    }
  }
}

void Net::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Net:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "min_x_:" << min_x_ << endl;
  os << string(indent_level * indent_size, ' ') << "min_y_:" << min_y_ << endl;
  os << string(indent_level * indent_size, ' ') << "max_x_:" << max_x_ << endl;
  os << string(indent_level * indent_size, ' ') << "max_y_:" << max_y_ << endl;
  os << string(indent_level * indent_size, ' ') << "macro_ids_: ";
  for (int macro_id : macro_ids_) {
    os << macro_id << " ";
  }
  os << endl;
}

int Net::num_macros() const { return macro_ids_.size(); }

int Net::macro_id(int nth_macro) const { return macro_ids_.at(nth_macro); }
