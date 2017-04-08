#include "./database.hpp"

using namespace std;

Database::Database(istream& block_input, istream& net_input) {
  ParseBlocks(block_input);
  ParseNets(net_input);
}

void Database::Print(ostream& os, int indent_level) const {
  const int indent_size = 2;
  os << string(indent_level * indent_size, ' ') << "Database:" << endl;
  ++indent_level;
  os << string(indent_level * indent_size, ' ')
     << "outline_width_: " << outline_width_ << endl;
  os << string(indent_level * indent_size, ' ')
     << "outline_height_: " << outline_height_ << endl;
  os << string(indent_level * indent_size, ' ') << "macros_:" << endl;
  ++indent_level;
  for (const Macro& macro : macros_) {
    macro.Print(os, indent_level);
  }
  --indent_level;
  os << string(indent_level * indent_size, ' ') << "terminals_:" << endl;
  ++indent_level;
  for (const Terminal& terminal : terminals_) {
    terminal.Print(os, indent_level);
  }
  --indent_level;
  os << string(indent_level * indent_size, ' ') << "nets_:" << endl;
  ++indent_level;
  for (const Net& net : nets_) {
    net.Print(os, indent_level);
  }
}

int Database::outline_width() const { return outline_width_; }

int Database::outline_height() const { return outline_height_; }

bool Database::is_macro_existed(const string& macro_name) const {
  return macro_id_from_name_.count(macro_name) == 1;
}

bool Database::is_terminal_existed(const string& terminal_name) const {
  return terminal_id_from_name_.count(terminal_name) == 1;
}

int Database::num_macros() const { return macros_.size(); }

int Database::num_terminals() const { return terminals_.size(); }

const Macro& Database::macro(int macro_id) const {
  return macros_.at(macro_id);
}

const Terminal& Database::terminal(int terminal_id) const {
  return terminals_.at(terminal_id);
}

int Database::macro_id_from_name(const string& macro_name) const {
  return macro_id_from_name_.at(macro_name);
}

int Database::terminal_id_from_name(const string& terminal_name) const {
  return terminal_id_from_name_.at(terminal_name);
}

void Database::ParseBlocks(istream& block_input) {
  simple_parser::Parser block_parser(block_input, ":");
  int num_macros = 0;
  int num_terminals = 0;
  int nth_macro = 0;
  int nth_terminal = 0;
  block_parser.Parse([&](const simple_parser::Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      string first_word = tokens[0];
      if (first_word == "Outline") {
        outline_width_ = stoi(tokens[1]);
        outline_height_ = stoi(tokens[2]);
      } else if (first_word == "NumBlocks") {
        num_macros = stoi(tokens[1]);
      } else if (first_word == "NumTerminals") {
        num_terminals = stoi(tokens[1]);
      } else {
        if (nth_macro < num_macros) {
          ++nth_macro;
          const string macro_name = tokens[0];
          const int macro_width = stoi(tokens[1]);
          const int macro_height = stoi(tokens[2]);
          macros_.push_back(Macro(macro_name, macro_width, macro_height));
          const int macro_id = macros_.size() - 1;
          macro_id_from_name_.insert({macro_name, macro_id});
        } else if (nth_terminal < num_terminals) {
          ++nth_terminal;
          const string terminal_name = tokens[0];
          const int terminal_x = stoi(tokens[2]);
          const int terminal_y = stoi(tokens[3]);
          terminals_.push_back(Terminal(terminal_name, terminal_x, terminal_y));
          const int terminal_id = terminals_.size() - 1;
          terminal_id_from_name_.insert({terminal_name, terminal_id});
        }
      }
    }
    return true;
  });
}
void Database::ParseNets(istream& net_input) {
  simple_parser::Parser net_parser(net_input, ":");
  int num_nets = 0;
  int nth_net = 0;
  net_parser.Parse([&](const simple_parser::Tokens& tokens) -> bool {
    if (!tokens.empty()) {
      string first_word = tokens[0];
      if (first_word == "NumNets") {
        num_nets = stoi(tokens[1]);
      } else if (first_word == "NetDegree") {
        ++nth_net;
        const int num_terminals = stoi(tokens[1]);
        int nth_terminal = 0;
        vector<int> net_macro_ids;
        vector<Terminal> net_terminals;
        net_parser.Parse([&](const simple_parser::Tokens& tokens) -> bool {
          if (!tokens.empty()) {
            ++nth_terminal;
            const string name = tokens[0];
            if (is_terminal_existed(name)) {
              net_terminals.push_back(terminal(terminal_id_from_name(name)));
            } else {
              net_macro_ids.push_back(macro_id_from_name(name));
            }
            if (nth_terminal == num_terminals) {
              return false;
            }
          }
          return true;
        });
        nets_.push_back(Net(net_macro_ids, net_terminals));
        if (nth_net == num_nets) {
          return false;
        }
      }
    }
    return true;
  });
}
