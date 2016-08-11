#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "b_star_tree.hpp"
#include "database.hpp"

#include <vector>

class Floorplanner {
 public:
  Floorplanner(const Database& database);

  int GetNumMacroInstances() const;
  std::string GetMacroInstanceName(int idx) const;
  int GetMacroInstanceX1(int idx) const;
  int GetMacroInstanceY1(int idx) const;
  int GetMacroInstanceX2(int idx) const;
  int GetMacroInstanceY2(int idx) const;

  int CalculateBestWidth() const;
  int CalculateBestHeight() const;
  float CalculateBestWireLength() const;

  void Floorplan();

 private:
  void Pack(BStarTree& floorplan);
  float CalculateWireLength(const BStarTree& floorplan) const;
  float Evaluate(const BStarTree& floorplan, float alpha) const;
  void SA();

  const Database& database_;
  BStarTree floorplan_;
  BStarTree best_floorplan_;
};

#endif
