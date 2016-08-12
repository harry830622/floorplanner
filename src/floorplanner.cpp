#include "floorplanner.hpp"

using namespace std;

Floorplanner::Floorplanner(const Database& database) : database_(database) {
  for (int i = 0; i < database.GetNumMacros(); ++i) {
    floorplan_.AddNewNode(i);
  }
  floorplan_.Skew();
}

void Floorplanner::Floorplan() {
}
