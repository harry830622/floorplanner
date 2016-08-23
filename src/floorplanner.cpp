#include "floorplanner.hpp"

using uni_database::UniDatabase;

Floorplanner::Floorplanner(const UniDatabase& database, double alpha)
    : database_(database), alpha_(alpha), best_floorplan_(database) {}

void Floorplanner::Run() {
  Floorplan floorplan(database_);
  for (int i = 0; i < 10000; ++i) {
    floorplan.Perturb();
  }
  floorplan.Print();
}
