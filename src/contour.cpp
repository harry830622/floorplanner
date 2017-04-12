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

tuple<Point, Point, list<Point>::iterator> Contour::Update(
    double macro_x, double macro_width, double macro_height) {
  return Update(macro_x, macro_width, macro_height, coordinates_.begin());
}

tuple<Point, Point, list<Point>::iterator> Contour::Update(
    double macro_x, double macro_width, double macro_height,
    list<Point>::iterator it_hint) {
  const double lower_left_x = macro_x;
  const double lower_left_y =
      FindMaxYBetween(macro_x, macro_x + macro_width, it_hint);
  const double upper_right_x = lower_left_x + macro_width;
  const double upper_right_y = lower_left_y + macro_height;
  if (upper_right_y > max_y_) {
    max_y_ = upper_right_y;
  }

  list<Point>::iterator it = it_hint;
  while (it != coordinates_.end() && it->x() < lower_left_x) {
    ++it;
  }
  list<Point>::iterator it_begin = it;
  list<Point>::iterator new_it_hint =
      (it_begin != coordinates_.begin()) ? prev(it_begin) : invalid_list_.end();
  while (it != coordinates_.end() && it->x() < upper_right_x) {
    ++it;
  }
  list<Point>::iterator it_end = it;

  const bool is_equal =
      (it_end == coordinates_.end() || it_end->x() != upper_right_x) ? false
                                                                     : true;
  const double last_y = prev(it_end)->y();

  coordinates_.erase(it_begin, it_end);

  coordinates_.insert(it_end, Point(lower_left_x, upper_right_y));
  if (!is_equal) {
    coordinates_.insert(it_end, Point(upper_right_x, last_y));
  }

  double previous_y = -1;
  for (it = coordinates_.begin(); it != coordinates_.end(); ++it) {
    if (it->y() == previous_y) {
      it = coordinates_.erase(it);
      --it;
    } else {
      previous_y = it->y();
    }
  }

  /* list<Point>::iterator it_rend = (new_it_hint == invalid_list_.end()) */
  /*                                     ? coordinates_.begin() */
  /*                                     : prev(new_it_hint); */
  /* it = prev(it_end); */
  /* while (it != it_rend) { */
  /*   if (it->y() != it_end->y()) { */
  /*     break; */
  /*   } */
  /*   if (it == new_it_hint) { */
  /*     new_it_hint = prev(new_it_hint); */
  /*   } */
  /*   it = coordinates_.erase(it); */
  /*   --it; */
  /* } */

  if (new_it_hint == invalid_list_.end()) {
    new_it_hint = coordinates_.begin();
  }

  return make_tuple(Point(lower_left_x, lower_left_y),
                    Point(upper_right_x, upper_right_y), new_it_hint);
}

// Private

list<Point> Contour::invalid_list_;

double Contour::FindMaxYBetween(double x_begin, double x_end,
                                list<Point>::iterator it_hint) const {
  list<Point>::const_iterator it =
      (it_hint != invalid_list_.end()) ? it_hint : coordinates_.begin();

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
