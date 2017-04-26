#include "./floorplanner.hpp"

#include <cmath>
#include <limits>

using namespace std;
using Json = nlohmann::json;

Floorplanner::Floorplanner(const Database& database, double alpha,
                           const string& sa_mode, bool is_drawing)
    : database_(database),
      alpha_(alpha),
      sa_mode_(sa_mode),
      is_drawing_(is_drawing),
      min_area_(numeric_limits<double>::max()),
      max_area_(0.0),
      min_wirelength_(numeric_limits<double>::max()),
      max_wirelength_(0.0),
      average_area_(0.0),
      average_wirelength_(0.0),
      average_uphill_cost_(0.0),
      best_floorplan_(Floorplan(database.num_macros(), is_drawing)),
      drawing_(Json::object()) {
  const int num_perturbations = 5000;
  Floorplan floorplan(best_floorplan_);

  double total_area = 0.0;
  double total_wirelength = 0.0;
  for (int i = 0; i < num_perturbations; ++i) {
    floorplan.Perturb(database_);
    floorplan.Pack(database_);
    const double area = floorplan.area();
    const double wirelength = floorplan.wirelength();
    if (area < min_area_) {
      min_area_ = area;
    }
    if (area > max_area_) {
      max_area_ = area;
    }
    if (wirelength < min_wirelength_) {
      min_wirelength_ = wirelength;
    }
    if (wirelength > max_wirelength_) {
      max_wirelength_ = wirelength;
    }
    total_area += area;
    total_wirelength += wirelength;
  }
  average_area_ = total_area / num_perturbations;
  average_wirelength_ = total_wirelength / num_perturbations;

  double total_uphill_cost = 0.0;
  int num_uphills = 0;
  double adaptive_alpha = alpha_ / 4.0;
  double adaptive_beta = (1.0 - alpha_) / 4.0;
  double last_cost =
      ComputeCost(best_floorplan_, adaptive_alpha, adaptive_beta);
  for (int i = 0; i < num_perturbations; ++i) {
    floorplan.Perturb(database_);
    floorplan.Pack(database_);
    double cost = ComputeCost(floorplan, adaptive_alpha, adaptive_beta);
    double delta_cost = cost - last_cost;
    if (delta_cost > 0) {
      ++num_uphills;
      total_uphill_cost += delta_cost;
    }
    last_cost = cost;
  }
  average_uphill_cost_ = total_uphill_cost / num_uphills;
}

const Json& Floorplanner::drawing() const { return drawing_; }

double Floorplanner::alpha() const { return alpha_; }

const Floorplan& Floorplanner::best_floorplan() const {
  return best_floorplan_;
}

void Floorplanner::Run() {
  if (sa_mode_ == "classical") {
    do {
      drawing_ = Json::object();

      SA();
    } while (best_floorplan_.area() == 0.0);
  } else if (sa_mode_ == "fast") {
    do {
      drawing_ = Json::object();

      FastSA();
    } while (best_floorplan_.area() == 0.0);
  } else {
    Floorplan initial_floorplan(best_floorplan_);

    do {
      drawing_ = Json::object();

      SA();
    } while (best_floorplan_.area() == 0.0);
    Floorplan floorplan_sa(best_floorplan_);

    best_floorplan_ = initial_floorplan;
    do {
      drawing_ = Json::object();

      FastSA();
    } while (best_floorplan_.area() == 0.0);
    Floorplan floorplan_fsa(best_floorplan_);

    const double beta = 1.0 - alpha_;
    if (ComputeCost(floorplan_sa, alpha_, beta) <
        ComputeCost(floorplan_fsa, alpha_, beta)) {
      best_floorplan_ = floorplan_sa;
    } else {
      best_floorplan_ = floorplan_fsa;
    }
  }
}

void Floorplanner::SA() {
  const double initial_uphill_probability = 0.999999;
  const double r = 0.85;
  const double initial_temperature =
      average_uphill_cost_ / -1 * log(initial_uphill_probability);
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 3;
  const double frozen_temperature = initial_temperature / 1.0e4;
  const double frozen_acceptance_rate = 0.01;
  const double alpha = alpha_;
  const double beta = (1 - alpha);
  const double adaptive_alpha_base = alpha / 4.0;
  const double adaptive_beta_base = beta / 4.0;

  if (is_drawing_) {
    drawing_["config"] = {{"initial_temperature", initial_temperature},
                          {"r", r},
                          {"num_perturbations", num_perturbations}};
    drawing_["iterations"] = Json::array();
  }

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);

  if (is_drawing_) {
    drawing_["initial_floorplan"] = floorplan.drawing();
  }

  double adaptive_alpha = adaptive_alpha_base;
  double adaptive_beta = adaptive_beta_base;
  double best_cost = ComputeCost(floorplan, adaptive_alpha, adaptive_beta);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  int nth_iteration = 0;
  while (temperature > frozen_temperature) {
    ++nth_iteration;

    int num_accepted_floorplans = 0;
    int num_feasible_floorplans = 0;

    if (is_drawing_) {
      Json iteration = Json::object();
      iteration["temperature"] = temperature;
      iteration["floorplans"] = Json::array();
      drawing_["iterations"].push_back(iteration);
    }

    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb(database_);
      new_floorplan.Pack(database_);

      const double cost =
          ComputeCost(new_floorplan, adaptive_alpha, adaptive_beta);
      const double delta_cost = cost - last_cost;
      if (delta_cost < 0) {
        ++num_accepted_floorplans;
        floorplan = new_floorplan;
        last_cost = cost;

        const double outline_width = database_.outline_width();
        const double outline_height = database_.outline_height();
        if (floorplan.width() <= outline_width &&
            floorplan.height() <= outline_height) {
          ++num_feasible_floorplans;
          if (cost < best_cost) {
            best_floorplan_ = floorplan;
            best_cost = cost;
          }
        }

        if (is_drawing_) {
          Json floorplan_drawing = floorplan.drawing();
          floorplan_drawing["is_accepted"] = true;
          floorplan_drawing["cost"] = cost;
          Json& iteration = drawing_["iterations"].back();
          iteration["floorplans"].push_back(floorplan_drawing);
        }
      } else {
        const double p = exp(-1 * delta_cost / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          ++num_accepted_floorplans;
          floorplan = new_floorplan;
          last_cost = cost;

          if (is_drawing_) {
            Json floorplan_drawing = floorplan.drawing();
            floorplan_drawing["is_accepted"] = true;
            floorplan_drawing["cost"] = cost;
            Json& iteration = drawing_["iterations"].back();
            iteration["floorplans"].push_back(floorplan_drawing);
          }
        } else {
          if (is_drawing_) {
            Json new_floorplan_drawing = new_floorplan.drawing();
            new_floorplan_drawing["is_accepted"] = false;
            new_floorplan_drawing["cost"] = cost;
            Json& iteration = drawing_["iterations"].back();
            iteration["floorplans"].push_back(new_floorplan_drawing);
          }
        }
      }
    }

    /* cout << "nth_iteration: " << nth_iteration << endl; */
    /* cout << "  temperature: " << temperature << endl; */
    /* cout << "  num_accepted_floorplans: " << num_accepted_floorplans << endl;
     */
    /* cout << "  num_feasible_floorplans: " << num_feasible_floorplans << endl;
     */
    /* cout << "  adaptive_alpha: " << adaptive_alpha */
    /*      << "\tadaptive_beta: " << adaptive_beta << endl; */
    /* cout << "  Best area: " << best_floorplan_.area() */
    /*      << "\tBest wirelength: " << best_floorplan_.wirelength() << endl; */

    if (num_accepted_floorplans / static_cast<double>(num_perturbations) <
        frozen_acceptance_rate) {
      break;
    }

    adaptive_alpha =
        adaptive_alpha_base +
        (alpha - adaptive_alpha_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));
    adaptive_beta =
        adaptive_beta_base +
        (beta - adaptive_beta_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));

    temperature *= r;

    if (is_drawing_) {
      Json& iteration = drawing_["iterations"].back();
      iteration["num_feasible_floorplans"] = num_feasible_floorplans;
    }
  }
}

void Floorplanner::FastSA() {
  const double initial_uphill_probability = 0.999999;
  const double initial_temperature =
      average_uphill_cost_ / -1 * log(initial_uphill_probability);
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 3;
  const double frozen_temperature = initial_temperature / 1.0e50;
  const double frozen_acceptance_rate = 0.01;
  const double alpha = alpha_;
  const double beta = (1 - alpha);
  const double adaptive_alpha_base = alpha / 4.0;
  const double adaptive_beta_base = beta / 4.0;
  const int c = 100;
  const int k = 6;

  if (is_drawing_) {
    drawing_["config"] = {{"initial_temperature", initial_temperature},
                          {"c", c},
                          {"k", k},
                          {"num_perturbations", num_perturbations}};
    drawing_["iterations"] = Json::array();
  }

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);

  if (is_drawing_) {
    drawing_["initial_floorplan"] = floorplan.drawing();
  }

  double adaptive_alpha = adaptive_alpha_base;
  double adaptive_beta = adaptive_beta_base;
  double best_cost = ComputeCost(floorplan, adaptive_alpha, adaptive_beta);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  int nth_iteration = 0;
  while (temperature > frozen_temperature) {
    ++nth_iteration;

    double total_delta_cost = 0.0;
    int num_accepted_floorplans = 0;
    int num_feasible_floorplans = 0;

    if (is_drawing_) {
      Json iteration = Json::object();
      iteration["temperature"] = temperature;
      iteration["floorplans"] = Json::array();
      drawing_["iterations"].push_back(iteration);
    }

    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb(database_);
      new_floorplan.Pack(database_);

      const double cost =
          ComputeCost(new_floorplan, adaptive_alpha, adaptive_beta);
      const double delta_cost = cost - last_cost;
      if (delta_cost < 0) {
        total_delta_cost += delta_cost;

        ++num_accepted_floorplans;
        floorplan = new_floorplan;
        last_cost = cost;

        const double outline_width = database_.outline_width();
        const double outline_height = database_.outline_height();
        if (new_floorplan.width() <= outline_width &&
            new_floorplan.height() <= outline_height) {
          ++num_feasible_floorplans;
          if (cost < best_cost) {
            best_floorplan_ = new_floorplan;
            best_cost = cost;
          }
        }

        if (is_drawing_) {
          Json floorplan_drawing = floorplan.drawing();
          floorplan_drawing["is_accepted"] = true;
          floorplan_drawing["cost"] = cost;
          Json& iteration = drawing_["iterations"].back();
          iteration["floorplans"].push_back(floorplan_drawing);
        }
      } else {
        const double p = exp(-1 * delta_cost / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          total_delta_cost += delta_cost;

          ++num_accepted_floorplans;
          floorplan = new_floorplan;
          last_cost = cost;

          if (is_drawing_) {
            Json floorplan_drawing = floorplan.drawing();
            floorplan_drawing["is_accepted"] = true;
            floorplan_drawing["cost"] = cost;
            Json& iteration = drawing_["iterations"].back();
            iteration["floorplans"].push_back(floorplan_drawing);
          }
        } else {
          if (is_drawing_) {
            Json new_floorplan_drawing = new_floorplan.drawing();
            new_floorplan_drawing["is_accepted"] = false;
            new_floorplan_drawing["cost"] = cost;
            Json& iteration = drawing_["iterations"].back();
            iteration["floorplans"].push_back(new_floorplan_drawing);
          }
        }
      }
    }

    /* cout << "nth_iteration: " << nth_iteration << endl; */
    /* cout << "  temperature: " << temperature << endl; */
    /* cout << "  total_delta_cost: " << total_delta_cost << endl; */
    /* cout << "  num_accepted_floorplans: " << num_accepted_floorplans << endl;
     */
    /* cout << "  num_feasible_floorplans: " << num_feasible_floorplans << endl;
     */
    /* cout << "  adaptive_alpha: " << adaptive_alpha */
    /*      << "\tadaptive_beta: " << adaptive_beta << endl; */
    /* cout << "  Best area: " << best_floorplan_.area() */
    /*      << "\tBest wirelength: " << best_floorplan_.wirelength() << endl; */

    if (num_accepted_floorplans / static_cast<double>(num_perturbations) <
        frozen_acceptance_rate) {
      break;
    }

    adaptive_alpha =
        adaptive_alpha_base +
        (alpha - adaptive_alpha_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));
    adaptive_beta =
        adaptive_beta_base +
        (beta - adaptive_beta_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));

    const double average_delta_cost = abs(total_delta_cost / num_perturbations);
    if (nth_iteration >= 1 && nth_iteration < k) {
      temperature =
          initial_temperature * average_delta_cost / (nth_iteration + 1) / c;
    } else {
      temperature =
          initial_temperature * average_delta_cost / (nth_iteration + 1);
    }

    if (is_drawing_) {
      Json& iteration = drawing_["iterations"].back();
      iteration["num_feasible_floorplans"] = num_feasible_floorplans;
    }
  }
}

double Floorplanner::ComputeCost(const Floorplan& floorplan, double alpha,
                                 double beta) const {
  const double outline_width = database_.outline_width();
  const double outline_height = database_.outline_height();
  const double width = floorplan.width();
  const double height = floorplan.height();

  const double penalty_weight = 10.0;
  double penalty = 0.0;
  if (width > outline_width || height > outline_height) {
    if (width > outline_width && height > outline_height) {
      penalty += (width * height - outline_width * outline_height);
    } else if (width > outline_width) {
      penalty += ((width - outline_width) * height);
    } else if (height > outline_height) {
      penalty += (width * (height - outline_height));
    }
    penalty += ((width - outline_width) * (width - outline_width) +
                (height - outline_height) * (height - outline_height));
    penalty /= (max_area_ - min_area_);
  }

  /* const double penalty_weight = 1.0; */
  /* double penalty = 0.0; */
  /* if (width > outline_width || height > outline_height) { */
  /*   const double ratio = height / width; */
  /*   const double outline_ratio = outline_height / outline_width; */
  /*   penalty = (ratio - outline_ratio) * (ratio - outline_ratio); */
  /* } */

  /* const double penalty_weight = 1.0; */
  /* const double ratio = height / width; */
  /* const double outline_ratio = outline_height / outline_width; */
  /* const double penalty = (ratio - outline_ratio) * (ratio - outline_ratio);
   */

  const double normalized_area = floorplan.area() / (max_area_ - min_area_);
  const double normalized_wirelength =
      floorplan.wirelength() / (max_wirelength_ - min_wirelength_);

  const double cost = alpha * normalized_area + beta * normalized_wirelength +
                      (1.0 - alpha - beta) * penalty_weight * penalty;

  return cost;
}
