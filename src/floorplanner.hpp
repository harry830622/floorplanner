#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "../lib/uni-database/uni_database.hpp"
#include "floorplan.hpp"

class Floorplanner {
 public:
  Floorplanner(const uni_database::UniDatabase& database, double alpha);
  Floorplanner& operator=(const Floorplanner& other) = delete;

  const Floorplan& best_floorplan() const;

  void Run();

 private:
  void SA(double initial_temperature = 10000.0, double r = 0.85);

  const uni_database::UniDatabase& database_;
  double alpha_;
  Floorplan best_floorplan_;
};

#endif
