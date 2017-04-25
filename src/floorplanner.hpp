#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "./floorplan.hpp"

#include "./json.hpp"

class Floorplanner {
 public:
  Floorplanner(const Database& database, double alpha,
               const std::string& sa_mode, bool is_drawing);

  const nlohmann::json& drawing() const;

  double alpha() const;
  const Floorplan& best_floorplan() const;

  void Run();

 private:
  void SA();
  void FastSA();
  double ComputeCost(const Floorplan& floorplan, double alpha,
                     double beta) const;

  const Database& database_;
  double alpha_;
  std::string sa_mode_;
  bool is_drawing_;
  double min_area_;
  double max_area_;
  double min_wirelength_;
  double max_wirelength_;
  double average_area_;
  double average_wirelength_;
  double average_uphill_cost_;
  Floorplan best_floorplan_;

  nlohmann::json drawing_;
};

#endif
