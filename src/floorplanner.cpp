#include "./floorplanner.hpp"

#include "./json.hpp"

#include <cmath>
#include <limits>

using namespace std;
using Json = nlohmann::json;

Floorplanner::Floorplanner(const Database& database, double alpha)
    : database_(database),
      alpha_(alpha),
      min_area_(numeric_limits<double>::max()),
      max_area_(0.0),
      min_wirelength_(numeric_limits<double>::max()),
      max_wirelength_(0.0),
      average_area_(0.0),
      average_wirelength_(0.0),
      average_uphill_cost_(0.0),
      best_floorplan_(Floorplan(database.num_macros())) {
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

void Floorplanner::Output(ostream& os, double runtime) const {
  const double width = best_floorplan_.width();
  const double height = best_floorplan_.height();
  const double area = best_floorplan_.area();
  const double wirelength = best_floorplan_.wirelength();
  os << alpha_ * area + (1 - alpha_) * wirelength << endl;
  os << wirelength << endl;
  os << area << endl;
  os << width << " " << height << endl;
  os << runtime << endl;
  for (int i = 0; i < database_.num_macros(); ++i) {
    const int macro_id = i;
    const string macro_name = database_.macro(macro_id).name();
    auto bounding_box = best_floorplan_.macro_bounding_box(macro_id);
    const Point& lower_left = bounding_box.first;
    const Point& upper_right = bounding_box.second;
    os << macro_name << " " << lower_left.x() << " " << lower_left.y() << " "
       << upper_right.x() << " " << upper_right.y() << endl;
  }
}

void Floorplanner::Draw(ostream& os) const {
  Json obj = Json::object();
  obj["objects"] = Json::array();
  for (int i = 0; i < database_.num_macros(); ++i) {
    const int macro_id = i;
    const string macro_name = database_.macro(macro_id).name();
    auto bounding_box = best_floorplan_.macro_bounding_box(macro_id);
    const Point& lower_left = bounding_box.first;
    const Point& upper_right = bounding_box.second;
    Json rect;
    rect["type"] = "rect";
    rect["color"] = 0xA9EEE6;
    rect["lower_left"] = {{"x", lower_left.x()}, {"y", lower_left.y()}};
    rect["upper_right"] = {{"x", upper_right.x()}, {"y", upper_right.y()}};
    obj["objects"].push_back(rect);
  }
  os << obj;
}

void Floorplanner::Run() {
  do {
    /* SA(); */
    FastSA();
  } while (best_floorplan_.area() == 0.0);

  const double outline_width = database_.outline_width();
  const double outline_height = database_.outline_height();
  const double best_width = best_floorplan_.width();
  const double best_height = best_floorplan_.height();
  const double best_area = best_floorplan_.area();
  const double best_wirelength = best_floorplan_.wirelength();
  const double best_cost = alpha_ * best_area + (1 - alpha_) * best_wirelength;

  cout << endl;
  cout << "============================= SUMMARY =============================="
       << endl;
  cout << "outline width:\t" << outline_width << "\t\toutline height:\t\t"
       << outline_height << endl;
  cout << "best width:\t" << best_width << "\t\tbest height:\t\t" << best_height
       << endl;
  cout << "best area:\t" << best_area << "\tbest wirelength:\t"
       << best_wirelength << endl;
  cout << "best cost:\t" << best_cost << endl;
}

void Floorplanner::SA() {
  const double initial_uphill_probability = 0.999;
  const double r = 0.85;
  const double initial_temperature =
      average_uphill_cost_ / -1 * log(initial_uphill_probability);
  const double frozen_temperature = initial_temperature / 10000.0;
  const int frozen_num_no_improvements = 10;
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 3;
  const double alpha = alpha_;
  const double beta = (1 - alpha);
  const double adaptive_alpha_base = alpha / 4.0;
  const double adaptive_beta_base = beta / 4.0;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double adaptive_alpha = adaptive_alpha_base;
  double adaptive_beta = adaptive_beta_base;
  double best_cost = ComputeCost(floorplan, adaptive_alpha, adaptive_beta);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  int num_no_improvements = 0;
  while (temperature > frozen_temperature &&
         num_no_improvements < frozen_num_no_improvements) {
    double total_delta_cost = 0.0;
    int num_feasible_floorplans = 0;

    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb(database_);
      new_floorplan.Pack(database_);

      const double cost =
          ComputeCost(new_floorplan, adaptive_alpha, adaptive_beta);
      const double delta_cost = cost - last_cost;
      if (delta_cost < 0) {
        total_delta_cost += delta_cost;

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
      } else {
        const double p = exp(-1 * delta_cost / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          total_delta_cost += delta_cost;

          floorplan = new_floorplan;
          last_cost = cost;
        }
      }
    }

    adaptive_alpha =
        adaptive_alpha_base +
        (alpha - adaptive_alpha_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));
    adaptive_beta =
        adaptive_beta_base +
        (beta - adaptive_beta_base) *
            (num_feasible_floorplans / static_cast<double>(num_perturbations));

    if (total_delta_cost == 0.0) {
      ++num_no_improvements;
    } else {
      num_no_improvements = 0;
    }

    temperature *= r;
  }
}

void Floorplanner::FastSA() {
  const double initial_uphill_probability = 0.999;
  const double initial_temperature =
      average_uphill_cost_ / -1 * log(initial_uphill_probability);
  const double frozen_temperature = initial_temperature / 1.0e50;
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 3;
  const double alpha = alpha_;
  const double beta = (1 - alpha);
  const double adaptive_alpha_base = alpha / 4.0;
  const double adaptive_beta_base = beta / 4.0;
  const int c = 100;
  const int k = 6;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double adaptive_alpha = adaptive_alpha_base;
  double adaptive_beta = adaptive_beta_base;
  double best_cost = ComputeCost(floorplan, adaptive_alpha, adaptive_beta);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  int nth_iteration = 0;
  while (temperature > frozen_temperature) {
    ++nth_iteration;

    double total_delta_cost = 0.0;
    int num_feasible_floorplans = 0;

    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb(database_);
      new_floorplan.Pack(database_);

      const double cost =
          ComputeCost(new_floorplan, adaptive_alpha, adaptive_beta);
      const double delta_cost = cost - last_cost;
      if (delta_cost < 0) {
        total_delta_cost += delta_cost;

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
      } else {
        const double p = exp(-1 * delta_cost / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          total_delta_cost += delta_cost;

          floorplan = new_floorplan;
          last_cost = cost;
        }
      }
    }

    /* cout << "nth_iteration: " << nth_iteration << endl; */
    /* cout << "  temperature: " << temperature << endl; */
    /* cout << "  total_delta_cost: " << total_delta_cost << endl; */
    /* cout << "  num_feasible_floorplans: " << num_feasible_floorplans << endl; */
    /* cout << "  adaptive_alpha: " << adaptive_alpha */
    /*      << "\tadaptive_beta: " << adaptive_beta << endl; */
    /* cout << "  Best area: " << best_floorplan_.area() */
    /*      << "\tBest wirelength: " << best_floorplan_.wirelength() << endl; */

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
  }
}

double Floorplanner::ComputeCost(const Floorplan& floorplan, double alpha,
                                 double beta) const {
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
    } else if (height > outline_height) {
      penalty += (width * (height - outline_height));
    }
    penalty += ((width - outline_width) * (width - outline_width) +
                (height - outline_height) * (height - outline_height));
    penalty /= (max_area_ - min_area_);
  }

  /* double penalty = 0.0; */
  /* if (width > outline_width || height > outline_height) { */
  /*   const double ratio = height / width; */
  /*   const double outline_ratio = outline_height / outline_width; */
  /*   penalty = (ratio - outline_ratio) * (ratio - outline_ratio); */
  /* } */

  /* const double ratio = height / width; */
  /* const double outline_ratio = outline_height / outline_width; */
  /* const double penalty = (ratio - outline_ratio) * (ratio - outline_ratio); */

  const double normalized_area = floorplan.area() / (max_area_ - min_area_);
  const double normalized_wirelength =
      floorplan.wirelength() / (max_wirelength_ - min_wirelength_);

  const double penalty_weight = 10.0;
  const double cost = alpha * normalized_area + beta * normalized_wirelength +
                      (1.0 - alpha - beta) * penalty_weight * penalty;

  return cost;
}
