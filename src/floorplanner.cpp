#include "floorplanner.hpp"

#include <stack>

using namespace std;

Floorplanner::Floorplanner(const Database& database) : database_(database) {
  for (int i = 0; i < database.GetNumMacros(); ++i) {
    floorplan_.AddNewNode(i);
  }
  floorplan_.Skew();
}

int Floorplanner::GetNumMacroInstances() const {}

string Floorplanner::GetMacroInstanceName(int idx) const {}

int Floorplanner::GetMacroInstanceX1(int idx) const {}

int Floorplanner::GetMacroInstanceY1(int idx) const {}

int Floorplanner::GetMacroInstanceX2(int idx) const {}

int Floorplanner::GetMacroInstanceY2(int idx) const {}

int Floorplanner::CalculateBestWidth() const {}

int Floorplanner::CalculateBestHeight() const {}

float Floorplanner::CalculateBestWireLength() const {}

void Floorplanner::Floorplan() {}

void Floorplanner::Pack(BStarTree& floorplan) {
}

float Floorplanner::CalculateWireLength(const BStarTree& floorplan) const {}

float Floorplanner::Evaluate(const BStarTree& floorplan, float alpha) const {}

void Floorplanner::SA() {}
