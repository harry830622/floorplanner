#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include "./point.hpp"

#include <string>

class Terminal {
 public:
  Terminal(const std::string& name, double x, double y);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  const std::string& name() const;
  const Point& coordinates() const;
  double x() const;
  double y() const;

 private:
  std::string name_;
  Point coordinates_;
};

#endif
