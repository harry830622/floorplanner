#ifndef NET_HPP
#define NET_HPP

#include "./terminal.hpp"

#include <vector>

class Net {
 public:
  Net(const std::vector<int>& macro_ids,
      const std::vector<Terminal>& terminals);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  int num_macros() const;
  int macro_id(int nth_macro) const;

 private:
  int min_x_;
  int min_y_;
  int max_x_;
  int max_y_;
  std::vector<int> macro_ids_;
};

#endif
