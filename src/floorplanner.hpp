#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "./floorplan.hpp"

class Floorplanner {
 public:
  Floorplanner(const Database& database, double alpha);

  void Run();

 private:
  const Database& database_;
  double alpha_;
  Floorplan best_floorplan_;
};

#endif
