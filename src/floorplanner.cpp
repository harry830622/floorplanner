#include "floorplanner.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

using uni_database::UniDatabase;
using uni_database::Keys;
using namespace std;

Floorplanner::Floorplanner(const UniDatabase& database, double alpha)
    : database_(database), alpha_(alpha), best_floorplan_(database) {}

const Floorplan& Floorplanner::best_floorplan() const {
  return best_floorplan_;
}

void Floorplanner::Run() {
  SA();
  /* FastSA(); */

  cout << database_.Data<int>(Keys{"outline", "width"}) << " "
       << database_.Data<int>(Keys{"outline", "height"}) << endl;
  cout << best_floorplan_.width() << " " << best_floorplan_.height() << endl;
}

void Floorplanner::SA() {
  const double initial_p = 0.99;
  const double r = 0.85;
  const double initial_temperature =
      best_floorplan_.average_uphill_cost() / -1 * log(initial_p);
  const double frozen_temperature = initial_temperature / 1000;
  const int num_perturbations =
      best_floorplan_.num_macros() * best_floorplan_.num_macros() * 3;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double best_cost = floorplan.Cost(database_, alpha_);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  while (temperature > frozen_temperature) {
    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb();
      new_floorplan.Pack(database_);

      double cost = new_floorplan.Cost(database_, alpha_);
      double cost_delta = cost - last_cost;
      if (cost_delta <= 0) {
        floorplan = new_floorplan;
        last_cost = cost;

        if (cost < best_cost) {
          int outline_width = database_.Data<int>(Keys{"outline", "width"});
          int outline_height = database_.Data<int>(Keys{"outline", "height"});
          if (new_floorplan.width() <= outline_width &&
              new_floorplan.height() <= outline_height) {
            best_floorplan_ = new_floorplan;
            best_cost = cost;
          }
        }
      } else {
        const double p = exp(-1 * cost_delta / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          floorplan = new_floorplan;
          last_cost = cost;
        }
      }
    }

    temperature *= r;
  }
}

void Floorplanner::FastSA() {
  const double initial_p = 0.99;
  const int c = 100;
  const int k = 7;
  const double initial_temperature =
      best_floorplan_.average_uphill_cost() / -1 * log(initial_p);
  const int num_perturbations =
      best_floorplan_.num_macros() * best_floorplan_.num_macros() * 3;
  const int frozen_worse = num_perturbations * 0.95;
  const double frozen_temperature = initial_temperature / 10000;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double best_cost = floorplan.Cost(database_, alpha_);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  double adaptive_alpha = alpha_;
  int num_iterations = 1;
  int num_worse = 0;
  while (num_worse < frozen_worse && temperature > frozen_temperature) {
    double total_cost_delta = 0.0;
    int num_feasible_solutions = 0;
    num_worse = 0;
    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb();
      new_floorplan.Pack(database_);

      int outline_width = database_.Data<int>(Keys{"outline", "width"});
      int outline_height = database_.Data<int>(Keys{"outline", "height"});
      bool is_feasible = false;
      if (new_floorplan.width() <= outline_width &&
          new_floorplan.height() <= outline_height) {
        is_feasible = true;
        ++num_feasible_solutions;
      }

      double cost = new_floorplan.Cost(database_, adaptive_alpha);
      double cost_delta = cost - last_cost;
      total_cost_delta += cost_delta;
      if (cost_delta <= 0) {
        floorplan = new_floorplan;
        last_cost = cost;
        if (cost < best_cost && is_feasible) {
          best_floorplan_ = new_floorplan;
          best_cost = cost;
        }
      } else {
        ++num_worse;
        const double p = exp(-1 * cost_delta / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          floorplan = new_floorplan;
          last_cost = cost;
        }
      }
    }

    /* adaptive_alpha = */
    /*     alpha_ + (1 - alpha_) * num_feasible_solutions / num_perturbations; */

    ++num_iterations;
    double average_cost_delta = total_cost_delta / num_perturbations;
    if (num_iterations >= 2 && num_iterations <= k) {
      temperature =
          initial_temperature * average_cost_delta / num_iterations / c;
    } else {
      temperature = initial_temperature * average_cost_delta / num_iterations;
    }
    /* cout << num_iterations << " " << temperature << " " << frozen_temperature */
    /*      << " " << num_worse << " " << frozen_worse << " " */
    /*      << num_feasible_solutions << " " << adaptive_alpha << endl; */
  }
}
