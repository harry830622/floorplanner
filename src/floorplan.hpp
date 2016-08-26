#ifndef FLOORPLAN_HPP
#define FLOORPLAN_HPP

#include "../lib/helpers/helpers.hpp"
#include "../lib/uni-database/uni_database.hpp"
#include "b_star_tree.hpp"

#include <functional>
#include <vector>

class Floorplan {
 public:
  Floorplan(const uni_database::UniDatabase& database);

  int width() const;
  int height() const;
  int num_macros() const;
  double average_uphill_cost() const;

  void Print(int indent = 0) const;

  int IterateMacros(
      std::function<void(int data_id, const helpers::Point<int>& coordinate,
                         bool is_rotated)>
          handler) const;
  double WireLength(const uni_database::UniDatabase& database) const;
  double Cost(const uni_database::UniDatabase& database,
              double alpha = 0.5) const;

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
  double average_area_;
  double average_wire_length_;
  double average_uphill_cost_;
  std::vector<Macro> macros_;
  std::vector<Net> nets_;
  BStarTree b_star_tree_;
};

#endif
