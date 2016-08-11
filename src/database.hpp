#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Database {
 public:
  Database() : outline_width_(0), outline_height_(0) {}

  int GetOutlineWidth() const;
  int GetOutlineHeight() const;

  int GetMacroIdx(const std::string& name) const;
  int GetNumMacros() const;
  std::string GetMacroName(int idx) const;
  int GetMacroWidth(int idx) const;
  int GetMacroHeight(int idx) const;

  int GetNumNets() const;
  int GetNumNetTerminals(int idx) const;
  int GetNetTerminalIdx(int idx, int terminal_idx) const;

  int GetTerminalIdx(const std::string& name) const;
  int GetNumTerminals() const;
  std::string GetTerminalName(int idx) const;
  float GetTerminalX(int idx) const;
  float GetTerminalY(int idx) const;

  void SetOutlineWidth(int width);
  void SetOutlineHeight(int height);

  int AddNewMacro(const std::string& name, int width, int height);
  int AddNewNet();
  int AddNewTerminal(const std::string& name, int x, int y);

  void AddNetTerminal(int idx, int terminal_idx);

 private:
  class Macro {
   public:
    Macro(const std::string& name, int width, int height)
        : name_(name), width_(width), height_(height) {}

    std::string name_;
    int width_, height_;
  };

  class Terminal {
   public:
    Terminal(const std::string& name, float x, float y) : name_(name), x_(x), y_(y) {}

    std::string name_;
    float x_, y_;
  };

  class Net {
   public:
    Net() = default;

    std::vector<int> terminal_idxs_;
  };

  int outline_width_, outline_height_;
  std::vector<Macro> macros_;
  std::map<std::string, int> macro_idxs_by_name_;
  std::vector<Terminal> terminals_;
  std::map<std::string, int> terminal_idxs_by_name_;
  std::vector<Net> nets_;
};

std::ostream& operator<<(std::ostream& os, const Database& database);

#endif
