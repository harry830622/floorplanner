#ifndef BTREE_HPP
#define BTREE_HPP

#include <vector>
#include "macro.hpp"
#include "net.hpp"
#include "contour.hpp"

using namespace std;

class BTree
{
  public:
    BTree(vector<Macro*>* macros, vector<Net*>* nets, Macro* root = 0);

    vector<Macro*>* macros_;
    vector<Net*>* nets_;
    Macro* root_;
    Contour contour_;
    int bounding_width_;
    int bounding_height_;
    int best_bounding_width_;
    int best_bounding_height_;

    void PrintMacros();
    void DeleteMacro(Macro* macro);
    void InsertMacro(Macro* macro_to_be_inserted, Macro* macro, int position);
    void SwapMacros(Macro* macro_a, Macro* macro_b);
    void Perturb();
    void SnapshotAllMacros();
    void RecoverAllMacros();
    void SnapshotAllMacrosBest();
    void RecoverAllMacrosBest();
    void SnapshotBestFloorplan();
    void RecoverBestFloorplan();
    void PackMacros();
    int CalculateArea();
    int CalculateWireLength();
    double CalculateCost(double alpha, double outline_ratio, double beta = 0);
    void SA(double alpha, int outline_width, int outline_height);
    void FSA(double alpha, int outline_width, int outline_height);
};

#endif
