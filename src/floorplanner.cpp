#include "./floorplanner.hpp"

Floorplanner::Floorplanner(const Database& database, double alpha)
    : database_(database),
      alpha_(alpha),
      best_floorplan_(Floorplan(database.num_macros())) {}

void Floorplanner::Run() {
  Floorplan floorplan(best_floorplan_);
  for (int i = 0; i < 1000000; ++i) {
    floorplan.Perturb();
  }
}
