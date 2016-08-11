#include "database.hpp"

using namespace std;

int Database::GetOutlineWidth() const { return outline_width_; }

int Database::GetOutlineHeight() const { return outline_height_; }

int Database::GetMacroIdx(const string& name) const {
  return macro_idxs_by_name_.at(name);
}

int Database::GetNumMacros() const { return macros_.size(); }

string Database::GetMacroName(int idx) const { return macros_.at(idx).name_; }

int Database::GetMacroWidth(int idx) const { return macros_.at(idx).width_; }

int Database::GetMacroHeight(int idx) const { return macros_.at(idx).height_; }

int Database::GetNumNets() const { return nets_.size(); }

int Database::GetNumNetTerminals(int idx) const {
  return nets_.at(idx).terminal_idxs_.size();
}

int Database::GetNetTerminalIdx(int idx, int terminal_idx) const {
  return nets_.at(idx).terminal_idxs_.at(terminal_idx);
}

int Database::GetTerminalIdx(const string& name) const {
  return terminal_idxs_by_name_.at(name);
}

int Database::GetNumTerminals() const { return terminals_.size(); }

string Database::GetTerminalName(int idx) const {
  return terminals_.at(idx).name_;
}

float Database::GetTerminalX(int idx) const { return terminals_.at(idx).x_; }

float Database::GetTerminalY(int idx) const { return terminals_.at(idx).y_; }

void Database::SetOutlineWidth(int width) { outline_width_ = width; }

void Database::SetOutlineHeight(int height) { outline_height_ = height; }

int Database::AddNewMacro(const string& name, int width, int height) {
  int idx = macros_.size();

  macros_.push_back(Macro(name, width, height));
  macro_idxs_by_name_.insert(make_pair(name, idx));

  int terminal_idx = terminals_.size();
  terminals_.push_back(Terminal(name, 0, 0));
  terminal_idxs_by_name_.insert(make_pair(name, terminal_idx));

  return idx;
}

int Database::AddNewNet() {
  int idx = nets_.size();

  nets_.push_back(Net());

  return idx;
}

int Database::AddNewTerminal(const string& name, int x, int y) {
  int idx = terminals_.size();

  terminals_.push_back(Terminal(name, x, y));
  terminal_idxs_by_name_.insert(make_pair(name, idx));

  return idx;
}

void Database::AddNetTerminal(int idx, int terminal_idx) {
  nets_.at(idx).terminal_idxs_.push_back(terminal_idx);
}

ostream& operator<<(ostream& os, const Database& database) {
  const int indent = 2;
  os << "Database:" << endl;
  os << string(indent, ' ') << "outline_width_: " << database.GetOutlineWidth()
     << endl;
  os << string(indent, ' ')
     << "outline_height_: " << database.GetOutlineHeight() << endl;
  os << string(indent, ' ') << "macros_:" << endl;
  for (int i = 0; i < database.GetNumMacros(); ++i) {
    cout << string(indent * 2, ' ') << "Macro: " << database.GetMacroName(i)
         << endl;
    cout << string(indent * 3, ' ') << "width_: " << database.GetMacroWidth(i)
         << endl;
    cout << string(indent * 3, ' ') << "height_: " << database.GetMacroHeight(i)
         << endl;
  }
  os << string(indent, ' ') << "terminals_:" << endl;
  for (int i = 0; i < database.GetNumTerminals(); ++i) {
    cout << string(indent * 2, ' ')
         << "Terminal: " << database.GetTerminalName(i) << endl;
    cout << string(indent * 3, ' ') << "x_: " << database.GetTerminalX(i)
         << endl;
    cout << string(indent * 3, ' ') << "y_: " << database.GetTerminalY(i)
         << endl;
  }
  os << string(indent, ' ') << "nets_:" << endl;
  for (int i = 0; i < database.GetNumNets(); ++i) {
    cout << string(indent * 2, ' ') << "Net:" << endl;
    cout << string(indent * 3, ' ') << "terminals:" << endl;
    for (int j = 0; j < database.GetNumNetTerminals(i); ++j) {
      int terminal_idx = database.GetNetTerminalIdx(i, j);
      cout << string(indent * 4, ' ') << database.GetTerminalName(terminal_idx)
           << endl;
    }
  }

  return os;
}
