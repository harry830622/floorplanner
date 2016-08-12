#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "database.hpp"
#include "floorplan.hpp"

#include <vector>

class Floorplanner {
 public:
  Floorplanner(const Database& database, float alpha)
      : database_(database),
        alpha_(alpha),
        floorplan_(database.GetNumMacros()),
        best_floorplan_(0) {}

  const Floorplan& GetBestFloorPlan() const;

  void Run();

 private:
  float CalculateWireLength(const Floorplan& floorplan) const;
  float Evaluate(const Floorplan& floorplan) const;

  void Pack(Floorplan& floorplan);
  void SA();

  const Database& database_;
  float alpha_;
  Floorplan floorplan_;
  Floorplan best_floorplan_;
};

#endif
