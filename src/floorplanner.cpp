#include "./floorplanner.hpp"

#include <cmath>

using namespace std;

Floorplanner::Floorplanner(const Database& database, double alpha)
    : database_(database),
      alpha_(alpha),
      average_area_(0.0),
      average_wirelength_(0.0),
      average_uphill_cost_(0.0),
      best_floorplan_(Floorplan(database.num_macros())) {
  const int num_perturbations = 1000;
  Floorplan floorplan(best_floorplan_);

  double total_area = 0.0;
  double total_wirelength = 0.0;
  for (int i = 0; i < num_perturbations; ++i) {
    floorplan.Perturb();
    floorplan.Pack(database_);
    total_area += floorplan.area();
    total_wirelength += floorplan.wirelength();
  }
  average_area_ = total_area / num_perturbations;
  average_wirelength_ = total_wirelength / num_perturbations;

  double total_uphill_cost = 0.0;
  int num_uphills = 0;
  double last_cost = ComputeCost(best_floorplan_);
  for (int i = 0; i < num_perturbations; ++i) {
    floorplan.Perturb();
    floorplan.Pack(database_);
    double cost = ComputeCost(floorplan);
    double cost_delta = cost - last_cost;
    if (cost_delta > 0) {
      ++num_uphills;
      total_uphill_cost += cost;
    }
    last_cost = cost;
  }
  average_uphill_cost_ = total_uphill_cost / num_uphills;
  /* cout << average_area_ << " " << average_wirelength_ << " " */
  /*      << average_uphill_cost_ << endl; */
}

void Floorplanner::Run() {
  SA();

  cout << endl;
  cout << database_.outline_width() << " " << database_.outline_height()
       << endl;
  cout << best_floorplan_.width() << " " << best_floorplan_.height() << endl;
}

void Floorplanner::SA() {
  const double initial_uphill_probability = 0.99;
  const double r = 0.85;
  const double initial_temperature =
      average_uphill_cost_ / -1 * log(initial_uphill_probability);
  const double frozen_temperature = initial_temperature / 1000;
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 3;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double best_cost = ComputeCost(floorplan);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  while (temperature > frozen_temperature) {
    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb();
      new_floorplan.Pack(database_);

      const double cost = ComputeCost(new_floorplan);
      const double cost_delta = cost - last_cost;
      if (cost_delta <= 0) {
        floorplan = new_floorplan;
        last_cost = cost;

        if (cost < best_cost) {
          const double outline_width = database_.outline_width();
          const double outline_height = database_.outline_height();
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

double Floorplanner::ComputeCost(const Floorplan& floorplan) const {
  const double outline_width = database_.outline_width();
  const double outline_height = database_.outline_height();
  const double width = floorplan.width();
  const double height = floorplan.height();

  double penalty = 0.0;
  if (width > outline_width || height > outline_height) {
    if (width > outline_width && height > outline_height) {
      penalty += (width * height - outline_width * outline_height);
    } else if (width > outline_width) {
      penalty += ((width - outline_width) * height);
      /* penalty += ((width - outline_width) * height + */
      /*             outline_width * (outline_height - height)); */
    } else if (height > outline_height) {
      penalty += (width * (height - outline_height));
      /* penalty += (width * (height - outline_height) + */
      /*             (outline_width - width) * outline_height); */
    }
    penalty += ((width - outline_width) * (width - outline_width) +
                (height - outline_height) * (height - outline_height));
    /* double ratio = height / static_cast<double>(width); */
    /* double outline_ratio = outline_height /
     * static_cast<double>(outline_width); */
    /* penalty += ((ratio - outline_ratio) * (ratio - outline_ratio)); */
  }

  const int penalty_weight = 10;
  const double cost =
      alpha_ * floorplan.area() / average_area_ +
      (1.0 - alpha_) * floorplan.wirelength() / average_wirelength_ +
      penalty_weight * penalty / average_area_;

  return cost;
}
