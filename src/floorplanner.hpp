#ifndef FLOORPLANNER_HPP
#define FLOORPLANNER_HPP

#include "./floorplan.hpp"

class Floorplanner {
 public:
  Floorplanner(const Database& database, double alpha);

  void Output(std::ostream& os = std::cout) const;
  void Draw(std::ostream& os = std::cout) const;

  void Run();

 private:
  void SA();
  double ComputeCost(const Floorplan& floorplan) const;

  const Database& database_;
  double alpha_;
  double average_area_;
  double average_wirelength_;
  double average_uphill_cost_;
  Floorplan best_floorplan_;
};

#endif
