#ifndef MACRO_HPP
#define MACRO_HPP

#include <string>
#include "point.hpp"
#include "terminal.hpp"

using namespace std;

class Macro
{
  public:
    Macro(string name, int width, int height, Terminal* terminal, bool is_rotatable = false);

    string name_;
    Point bottom_left_;
    int width_;
    int height_;
    Terminal* terminal_;
    bool is_rotatable_;
    bool is_visited_;
    Macro* parent_;
    Macro* left_child_;
    Macro* right_child_;
    Point old_bottom_left_;
    int old_width_;
    int old_height_;
    Macro* old_parent_;
    Macro* old_left_child_;
    Macro* old_right_child_;
    Point best_bottom_left_;
    int best_width_;
    int best_height_;
    Macro* best_parent_;
    Macro* best_left_child_;
    Macro* best_right_child_;

    int GetX();
    int GetY();
    Point GetCenterPoint();
    void SetX(int x);
    void SetY(int y);
    void Rotate();
    void Snapshot();
    void RecoverSnapshot();
    void SnapshotBest();
    void RecoverSnapshotBest();
};

#endif
