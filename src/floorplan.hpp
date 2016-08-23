#ifndef FLOORPLAN_HPP
#define FLOORPLAN_HPP

#include "../lib/helpers/helpers.hpp"
#include "../lib/uni-database/uni_database.hpp"
#include "b_star_tree.hpp"

#include <vector>

class Floorplan {
 public:
  Floorplan(const uni_database::UniDatabase& database);

  int width() const;
  int height() const;

  double WireLength(const uni_database::UniDatabase& database) const;

  void Print(int indent = 0) const;

  void Perturb();
  void Pack(const uni_database::UniDatabase& database);

 private:
  class Macro {
   public:
    Macro(int data_id);

    int data_id_;
    helpers::Point<int> coordinate_;
    bool is_rotated_;
  };

  class Net {
   public:
    Net(int data_id);

    int data_id_;
    helpers::Rect<int> bounding_box_;
    std::vector<int> macro_ids_;
  };

  int width_;
  int height_;
  std::vector<Macro> macros_;
  std::vector<Net> nets_;
  BStarTree b_star_tree_;
};

#endif
