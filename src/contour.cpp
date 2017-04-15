#include "./contour.hpp"

using namespace std;

Contour::Contour() : max_y_(0.0), coordinates_(1, Point(0.0, 0.0)) {}

void Contour::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Contour:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ') << "max_y_: " << max_y_ << endl;
  os << string(indent_level * indent_size, ' ') << "coordinates_:" << endl;
  ++indent_level;
  for (const Point& point : coordinates_) {
    point.Print(os, indent_level);
  }
}

double Contour::max_x() const { return coordinates_.back().x(); }

double Contour::max_y() const { return max_y_; }

pair<Point, Point> Contour::Update(double macro_x, double macro_width,
                                   double macro_height) {
  const double lower_left_x = macro_x;
  const double lower_left_y = FindMaxYBetween(macro_x, macro_x + macro_width);
  const double upper_right_x = lower_left_x + macro_width;
  const double upper_right_y = lower_left_y + macro_height;
  if (upper_right_y > max_y_) {
    max_y_ = upper_right_y;
  }

  list<Point>::iterator it = coordinates_.begin();
  while (it != coordinates_.end() && it->x() < lower_left_x) {
    ++it;
  }
  list<Point>::iterator it_begin = it;
  while (it != coordinates_.end() && it->x() < upper_right_x) {
    ++it;
  }
  list<Point>::iterator it_end = it;

  const bool is_touched =
      (it_end == coordinates_.end() || it_end->x() != upper_right_x) ? false
                                                                     : true;
  const double last_y = prev(it_end)->y();

  bool is_previous_y_equal = false;
  if (it_begin != coordinates_.begin() &&
      prev(it_begin)->y() == upper_right_y) {
    is_previous_y_equal = true;
  }

  bool is_next_y_equal = false;
  if (it_end != coordinates_.end() && it_end->y() == upper_right_y) {
    is_next_y_equal = true;
  }

  bool is_last_y_next_y_equal = false;
  if (it_end != coordinates_.end() && it_end->y() == last_y) {
    is_last_y_next_y_equal = true;
  }

  if (is_previous_y_equal) {
    coordinates_.erase(it_begin, it_end);
  } else {
    coordinates_.erase(it_begin, it_end);
    coordinates_.insert(it_end, Point(lower_left_x, upper_right_y));
  }
  if (is_touched && is_next_y_equal) {
    coordinates_.erase(it_end);
  } else {
    coordinates_.insert(it_end, Point(upper_right_x, last_y));
    if (is_last_y_next_y_equal) {
      coordinates_.erase(it_end);
    }
  }

  /* double previous_y = -1; */
  /* for (it = coordinates_.begin(); it != coordinates_.end(); ++it) { */
  /*   if (it->y() == previous_y) { */
  /*     it = coordinates_.erase(it); */
  /*     --it; */
  /*   } else { */
  /*     previous_y = it->y(); */
  /*   } */
  /* } */

  return make_pair(Point(lower_left_x, lower_left_y),
                   Point(upper_right_x, upper_right_y));
}

// Private

double Contour::FindMaxYBetween(double x_begin, double x_end) const {
  list<Point>::const_iterator it = coordinates_.begin();

  while (it != coordinates_.end() && it->x() <= x_begin) {
    ++it;
  }
  --it;

  double max_y = 0;
  while (it != coordinates_.end() && it->x() < x_end) {
    if (it->y() > max_y) {
      max_y = it->y();
    }
    ++it;
  }

  return max_y;
}
