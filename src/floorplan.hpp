#ifndef FLOORPLAN_HPP
#define FLOORPLAN_HPP

#include "./b_star_tree.hpp"
#include "./contour.hpp"
#include "./database.hpp"

class Floorplan {
 public:
  Floorplan(int num_macros);

  double area() const;
  double wirelength() const;

  void Perturb();
  void Pack(const Database& database);

 private:
  double area_;
  double wirelength_;
  BStarTree b_star_tree_;
  std::vector<int> macro_id_from_node_id_;
  std::vector<bool> is_rotated_from_macro_id_;
  Contour contour_;
  std::vector<std::pair<Point, Point>> macro_coordinates_from_macro_id_;
};

#endif
