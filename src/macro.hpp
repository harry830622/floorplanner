#ifndef MACRO_HPP
#define MACRO_HPP

#include <iostream>
#include <string>

class Macro {
 public:
  Macro(const std::string& name, int width, int height);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  const std::string& name() const;
  int width() const;
  int height() const;

 private:
  std::string name_;
  int width_;
  int height_;
};

#endif
