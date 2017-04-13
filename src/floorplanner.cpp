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
  const int num_perturbations = 1000;
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
  double last_cost = ComputeCost(best_floorplan_);
  for (int i = 0; i < num_perturbations; ++i) {
    floorplan.Perturb(database_);
    floorplan.Pack(database_);
    double cost = ComputeCost(floorplan);
    double delta_cost = cost - last_cost;
    if (delta_cost > 0) {
      ++num_uphills;
      total_uphill_cost += delta_cost;
    }
    last_cost = cost;
  }
  average_uphill_cost_ = total_uphill_cost / num_uphills;
  /* cout << average_area_ << " " << average_wirelength_ << " " */
  /*      << average_uphill_cost_ << endl; */
}

void Floorplanner::Output(ostream& os, double runtime) const {
  const double width = best_floorplan_.width();
  const double height = best_floorplan_.height();
  const double area = best_floorplan_.area();
  const double wirelength = best_floorplan_.wirelength();
  os << alpha_ * area + (1 - alpha_) * wirelength << endl;
  os << wirelength << endl;
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
  const double frozen_temperature = initial_temperature / 1000.0;
  const int frozen_num_no_improvements = 10;
  const int num_perturbations =
      database_.num_macros() * database_.num_macros() * 5;

  Floorplan floorplan(best_floorplan_);
  floorplan.Pack(database_);
  double best_cost = ComputeCost(floorplan);
  double last_cost = best_cost;
  double temperature = initial_temperature;
  int num_no_improvements = 0;
  while (temperature > frozen_temperature ||
         num_no_improvements < frozen_num_no_improvements) {
    double total_delta_cost = 0.0;

    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan(floorplan);
      new_floorplan.Perturb(database_);
      new_floorplan.Pack(database_);

      const double cost = ComputeCost(new_floorplan);
      const double delta_cost = cost - last_cost;
      if (delta_cost <= 0) {
        total_delta_cost += delta_cost;

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
        const double p = exp(-1 * delta_cost / temperature);
        if (rand() / static_cast<double>(RAND_MAX) < p) {
          total_delta_cost += delta_cost;

          floorplan = new_floorplan;
          last_cost = cost;
        }
      }
    }

    cout << "temperature: " << temperature
         << "\ttotal_delta_cost: " << total_delta_cost << endl;
    cout << "best_width: " << best_floorplan_.width()
         << "\tbest_height: " << best_floorplan_.height() << endl;

    if (total_delta_cost == 0.0) {
      ++num_no_improvements;
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
    /* double ratio = height / width; */
    /* double outline_ratio = outline_height / outline_width; */
    /* penalty += ((ratio - outline_ratio) * (ratio - outline_ratio)); */
  }

  const int penalty_weight = 10;
  const double cost = alpha_ * floorplan.area() / (max_area_ - min_area_) +
                      (1.0 - alpha_) * floorplan.wirelength() /
                          (max_wirelength_ - min_wirelength_) +
                      penalty_weight * penalty / (max_area_ - min_area_);
  /* penalty_weight * penalty; */

  return cost;
}
