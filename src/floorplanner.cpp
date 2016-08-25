#include "floorplanner.hpp"

#include <cmath>
#include <cstdlib>

using uni_database::UniDatabase;

Floorplanner::Floorplanner(const UniDatabase& database, double alpha)
    : database_(database), alpha_(alpha), best_floorplan_(database) {}

const Floorplan& Floorplanner::best_floorplan() const {
  return best_floorplan_;
}

void Floorplanner::Run() { SA(); }

void Floorplanner::SA() {
  const double initial_temperature = 10000.0;
  const double r = 0.85;
  const int num_perturbations =
      best_floorplan_.num_macros() * best_floorplan_.num_macros() * 3;

  Floorplan& floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double last_cost = floorplan.Cost(database_, alpha_);
  double temperature = initial_temperature;
  while (temperature > 1) {
    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb();
      new_floorplan.Pack(database_);

      double cost = new_floorplan.Cost(database_, alpha_);
      double cost_delta = cost - last_cost;
      if (cost_delta < 0 || (cost_delta > 0 &&
                             rand() / static_cast<double>(RAND_MAX) <
                                 exp(-1 * cost_delta / temperature))) {
        floorplan = new_floorplan;
        last_cost = cost;

        if (cost_delta < 0 && cost < best_floorplan_.Cost(database_, alpha_)) {
          best_floorplan_ = new_floorplan;
        }
      }
    }

    temperature *= r;
  }
}
