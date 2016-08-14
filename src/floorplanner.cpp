#include "floorplanner.hpp"
#include "contour.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>

using namespace std;

const Floorplan& Floorplanner::GetBestFloorPlan() const {
  return best_floorplan_;
}

float Floorplanner::CalculateBestWireLength() const {
  return CalculateWireLength(best_floorplan_);
}

void Floorplanner::Run() { SA(); }

float Floorplanner::CalculateWireLength(const Floorplan& floorplan) const {
  float wire_length = 0.0;

  for (int i = 0; i < database_.GetNumNets(); ++i) {
    float x1 = numeric_limits<float>::max();
    float y1 = numeric_limits<float>::max();
    float x2 = 0.0;
    float y2 = 0.0;
    for (int j = 0; j < database_.GetNumNetTerminals(i); ++j) {
      int terminal_idx = database_.GetNetTerminalIdx(i, j);
      string terminal_name = database_.GetTerminalName(terminal_idx);
      float x, y;
      if (database_.HasMacro(terminal_name)) {
        int macro_idx = database_.GetMacroIdx(terminal_name);
        int macro_instance_width = database_.GetMacroWidth(macro_idx);
        int macro_instance_height = database_.GetMacroHeight(macro_idx);
        int macro_instance_idx = macro_idx;
        int macro_instance_x = floorplan.GetMacroInstanceX(macro_instance_idx);
        int macro_instance_y = floorplan.GetMacroInstanceY(macro_instance_idx);
        bool is_macro_instance_rotated =
            floorplan.GetMacroInstanceIsRotated(macro_instance_idx);
        if (is_macro_instance_rotated) {
          swap(macro_instance_width, macro_instance_height);
        }
        x = macro_instance_x + macro_instance_width / 2.0;
        y = macro_instance_y + macro_instance_height / 2.0;
      } else {
        x = static_cast<float>(database_.GetTerminalX(terminal_idx));
        y = static_cast<float>(database_.GetTerminalY(terminal_idx));
      }
      x1 = min(x1, x);
      y1 = min(y1, y);
      x2 = max(x2, x);
      y2 = max(y2, y);
    }
    wire_length += (x2 - x1 + y2 - y1);
  }

  return wire_length;
}

float Floorplanner::Evaluate(const Floorplan& floorplan) {
  const int outline_width = database_.GetOutlineWidth();
  const int outline_height = database_.GetOutlineHeight();
  const int width = floorplan.GetWidth();
  const int height = floorplan.GetHeight();

  /* const float k = 1.0; */
  /* const float c_width = */
  /*     (width - outline_width) / static_cast<float>(outline_width); */
  /* const float c_height = */
  /*     (height - outline_height) / static_cast<float>(outline_height); */
  /* float cost = width * height + */
  /*              k / 2 * (c_width * c_width + c_height * c_height) - */
  /*              (lambda_width_ * c_width + lambda_height_ * c_height); */
  /* lambda_width_ = lambda_width_ - k * c_width; */
  /* lambda_height_ = lambda_height_ - k * c_height; */

  /* const float penalty = 10000; */
  /* float cost = width * height + */
  /*              penalty * (width - outline_width) * (width - outline_width) +
   */
  /*              penalty * (height - outline_height) * (height -
   * outline_height); */

  /* const float outline_ratio = */
  /*     outline_height / static_cast<float>(outline_width); */
  /* const float ratio = height / static_cast<float>(width); */
  /* float cost = width * height + */
  /*              100000000 * (ratio - outline_ratio) * (ratio - outline_ratio);
   */

  float cost = width * height +
               (width - outline_width) * (width - outline_width) +
               (height - outline_height) * (height - outline_height);

  return cost;
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

void Floorplanner::SA() {
  const float initial_temperature = 10000000.0;
  const float r = 0.85;
  const int num_perturbations = floorplan_.GetNumMacroInstances() * 100;

  Pack(floorplan_);
  best_floorplan_ = floorplan_;
  /* lambda_width_ = 1.0; */
  /* lambda_height_ = 1.0; */
  float last_cost = Evaluate(floorplan_);
  float temperature = initial_temperature;
  while (temperature > 1) {
    for (int i = 0; i < num_perturbations; ++i) {
      Floorplan new_floorplan = floorplan_.Perturb();
      Pack(new_floorplan);

      float cost = Evaluate(new_floorplan);
      float cost_delta = cost - last_cost;
      if (cost_delta < 0 || (cost_delta > 0 &&
                             rand() / static_cast<float>(RAND_MAX) <
                                 exp(-1 * cost_delta / temperature))) {
        floorplan_ = new_floorplan;
        last_cost = cost;
      }
      if (cost < Evaluate(best_floorplan_)) {
        best_floorplan_ = new_floorplan;
      }
    }

    temperature *= r;
  }
}
