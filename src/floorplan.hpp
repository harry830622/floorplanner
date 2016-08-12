#ifndef FLOORPLAN_HPP
#define FLOORPLAN_HPP

#include "b_star_tree.hpp"

#include <vector>

class Floorplan {
 public:
  Floorplan(int num_macros);
  Floorplan(const Floorplan& floorplan) = default;
  Floorplan& operator=(const Floorplan& floorplan) = default;

  const BStarTree& GetBStarTree() const;

  int GetNumMacroInstances() const;
  int GetMacroInstanceMacroIdx(int idx) const;
  int GetMacroInstanceX(int idx) const;
  int GetMacroInstanceY(int idx) const;
  int GetMacroInstanceIsRotated(int idx) const;

  int CalculateBestWidth() const;
  int CalculateBestHeight() const;

  Floorplan Perturb() const;

  void SetMacroInstanceX(int idx, int x);
  void SetMacroInstanceY(int idx, int y);

 private:
  class MacroInstance {
   public:
    MacroInstance(int macro_idx)
        : macro_idx_(macro_idx), x_(0), y_(0), is_rotated_(false) {}
    MacroInstance(const MacroInstance& macro_instance) = default;
    MacroInstance& operator=(const MacroInstance& macro_instance) = default;

    int macro_idx_;
    int x_;
    int y_;
    bool is_rotated_;
  };

  BStarTree b_star_tree_;
  std::vector<MacroInstance> macro_instances_;
};

std::ostream& operator<<(std::ostream& os, const Floorplan& floorplan);

#endif
