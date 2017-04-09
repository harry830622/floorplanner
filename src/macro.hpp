#ifndef MACRO_HPP
#define MACRO_HPP

#include <iostream>
#include <string>

class Macro {
 public:
  Macro(const std::string& name, double width, double height);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  const std::string& name() const;
  double width() const;
  double height() const;

 private:
  std::string name_;
  double width_;
  double height_;
};

#endif
