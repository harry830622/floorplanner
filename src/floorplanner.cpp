#include "floorplanner.hpp"
#include "contour.hpp"

#include <cassert>

using namespace std;

const Floorplan& Floorplanner::GetBestFloorPlan() const {
  return best_floorplan_;
}

void Floorplanner::Run() {
  for (int i = 0; i < 1000; ++i) {
    floorplan_ = floorplan_.Perturb();
  }
  Pack(floorplan_);
  best_floorplan_ = floorplan_;
}

void Floorplanner::Pack(Floorplan& floorplan) {
  Contour contour;
  floorplan.DfsBStarTree([this, &floorplan, &contour](
      int current_idx, int parent_idx, char position) {
    const Database& database = this->database_;
    const BStarTree& b_star_tree = floorplan.GetBStarTree();
    int macro_instance_idx = b_star_tree.GetNodeMacroInstanceIdx(current_idx);
    int macro_idx = floorplan.GetMacroInstanceMacroIdx(macro_instance_idx);
    int macro_instance_width = database.GetMacroWidth(macro_idx);
    int macro_instance_height = database.GetMacroHeight(macro_idx);
    bool is_macro_instance_rotated =
        floorplan.GetMacroInstanceIsRotated(macro_instance_idx);
    if (is_macro_instance_rotated) {
      swap(macro_instance_width, macro_instance_height);
    }

    if (parent_idx == -1) {
      floorplan.SetMacroInstanceX(macro_instance_idx, 0);
      floorplan.SetMacroInstanceY(macro_instance_idx, 0);
      contour.Update(0, macro_instance_width, macro_instance_height);
    } else {
      int parent_macro_instance_idx =
          b_star_tree.GetNodeMacroInstanceIdx(parent_idx);
      int parent_macro_idx =
          floorplan.GetMacroInstanceMacroIdx(parent_macro_instance_idx);
      int parent_macro_instance_x =
          floorplan.GetMacroInstanceX(parent_macro_instance_idx);
      int parent_macro_instance_width =
          database.GetMacroWidth(parent_macro_idx);
      bool is_parent_macro_instance_rotated =
          floorplan.GetMacroInstanceIsRotated(parent_macro_instance_idx);
      if (is_parent_macro_instance_rotated) {
        parent_macro_instance_width = database.GetMacroHeight(parent_macro_idx);
      }
      switch (position) {
        case 'L': {
          int x = parent_macro_instance_x + parent_macro_instance_width;
          floorplan.SetMacroInstanceX(macro_instance_idx, x);
          floorplan.SetMacroInstanceY(
              macro_instance_idx,
              contour.FindMaxYBetween(x, x + macro_instance_width));

          contour.Update(x, macro_instance_width, macro_instance_height);
          break;
        }
        case 'R': {
          int x = parent_macro_instance_x;
          floorplan.SetMacroInstanceX(macro_instance_idx, x);
          floorplan.SetMacroInstanceY(
              macro_instance_idx,
              contour.FindMaxYBetween(x, x + macro_instance_width));
          contour.Update(x, macro_instance_width, macro_instance_height);
          break;
        }
        default:
          break;
      }
    }
  });
  floorplan.SetWidth(contour.FindMaxX());
  floorplan.SetHeight(contour.FindMaxY());
}

void Floorplanner::SA() {}
