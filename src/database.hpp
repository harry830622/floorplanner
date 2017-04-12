#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "./macro.hpp"
#include "./net.hpp"
#include "./simple_parser.hpp"
#include "./terminal.hpp"

#include <map>

class Database {
 public:
  Database(std::istream& block_input, std::istream& net_input);

  void Print(std::ostream& os = std::cout, int indent_level = 0) const;

  double outline_width() const;
  double outline_height() const;
  bool is_macro_existed(const std::string& macro_name) const;
  bool is_terminal_existed(const std::string& terminal_name) const;
  int num_macros() const;
  int num_terminals() const;
  int num_nets() const;
  const Macro& macro(int macro_id) const;
  const Terminal& terminal(int terminal_id) const;
  const Net& net(int net_id) const;
  int macro_id_from_name(const std::string& macro_name) const;
  int terminal_id_from_name(const std::string& terminal_name) const;
  bool is_macro_rotatable(int macro_id) const;

 private:
  void ParseBlocks(std::istream& block_input);
  void ParseNets(std::istream& net_input);

  double outline_width_;
  double outline_height_;
  std::vector<Macro> macros_;
  std::vector<Terminal> terminals_;
  std::vector<Net> nets_;
  std::map<std::string, int> macro_id_from_name_;
  std::map<std::string, int> terminal_id_from_name_;
  std::vector<bool> is_macro_rotatable_from_id_;
};

#endif
