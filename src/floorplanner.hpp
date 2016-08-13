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
        /* lambda_width_(0.0), */
        /* lambda_height_(0.0), */
        floorplan_(database.GetNumMacros()),
        best_floorplan_(0) {}

  const Floorplan& GetBestFloorPlan() const;

  float CalculateBestWireLength() const;

  void Run();

 private:
  float CalculateWireLength(const Floorplan& floorplan) const;
  float Evaluate(const Floorplan& floorplan);

  void Pack(Floorplan& floorplan);
  void SA();

  const Database& database_;
  float alpha_;
  /* float lambda_width_; */
  /* float lambda_height_; */
  Floorplan floorplan_;
  Floorplan best_floorplan_;
};

#endif
