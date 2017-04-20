#ifndef FLOORPLAN_HPP
#define FLOORPLAN_HPP

#include "./b_star_tree.hpp"
#include "./database.hpp"

class Floorplan {
 public:
  Floorplan(int num_macros);

  int num_macros() const;
  double width() const;
  double height() const;
  double area() const;
  double wirelength() const;
  const std::pair<Point, Point>& macro_bounding_box(int macro_id) const;

  void Perturb(const Database& database);
  void Pack(const Database& database);

 private:
  double width_;
  double height_;
  double wirelength_;
  BStarTree b_star_tree_;
  std::vector<int> macro_id_by_node_id_;
  std::vector<bool> is_macro_rotated_by_id_;
  std::vector<std::pair<Point, Point>> macro_bounding_box_by_id_;
};

#endif
