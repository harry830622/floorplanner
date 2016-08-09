#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>
#include <map>
#include "macro.hpp"
#include "terminal.hpp"
#include "net.hpp"
#include "btree.hpp"

using namespace std;

int ConvertStringToInt(string str);

int main(int argc, char* argv[])
{

  // Check if the number of the arguments is correct.
  if (argc != 6) {
    cout << "Usage: FP [alpha] [block input] [net input] [output] [mode]\n";
    return 1;
  }

  double alpha = atof(argv[1]);
  if (alpha < 0 || alpha > 1) {
    cout << "Alpha value must be in [0, 1]\n";
    return 2;
  }
  ifstream block_input(argv[2]);
  ifstream net_input(argv[3]);
  ofstream output(argv[4]);

  clock_t begin_time = clock();

  // Parsing block input.
  int outline_width, outline_height;
  int num_of_blocks;
  int num_of_terminals;
  vector<Macro*> all_macros;
  map<string, Terminal*> all_terminals;
  string tmp;
  block_input >> tmp; //for "Outline:"
  block_input >> tmp;
  outline_width = ConvertStringToInt(tmp);
  block_input >> tmp;
  outline_height = ConvertStringToInt(tmp);
  block_input >> tmp; //for "NumBlocks:"
  block_input >> tmp;
  num_of_blocks = ConvertStringToInt(tmp);
  block_input >> tmp; //for "NumTerminals:"
  block_input >> tmp;
  num_of_terminals = ConvertStringToInt(tmp);
  for (int i = 0; i < num_of_blocks; ++i) {
    string name;
    int w, h;
    block_input >> name;
    block_input >> tmp;
    w = ConvertStringToInt(tmp);
    block_input >> tmp;
    h = ConvertStringToInt(tmp);

    if (w > outline_width) {
      int tmp = w;
      w = h;
      h = tmp;
    }
    assert(w <= outline_width);
    bool is_rotatable = false;
    if (w <= outline_height && h <= outline_width) {
      is_rotatable = true;
    }

    Terminal* t = new Terminal(name);
    all_terminals.insert(pair<string, Terminal*>(name, t));
    Macro* m = new Macro(name, w, h, t, is_rotatable);
    all_macros.push_back(m);
  }
  for (int i = 0; i < num_of_terminals; ++i) {
    string name;
    int x, y;
    block_input >> name;
    block_input >> tmp; //for "terminal"
    block_input >> tmp;
    x = ConvertStringToInt(tmp);
    block_input >> tmp;
    y = ConvertStringToInt(tmp);
    Terminal* t = new Terminal(name, x, y);
    all_terminals.insert(pair<string, Terminal*>(name, t));
  }

  // Parsing net input.
  int num_of_nets;
  vector<Net*> all_nets;
  net_input >> tmp; //for "NumNets:"
  net_input >> tmp;
  num_of_nets = ConvertStringToInt(tmp);
  for (int i = 0; i < num_of_nets; ++i) {
    int net_degree;
    net_input >> tmp; //for "NetDegree:"
    net_input >> tmp;
    net_degree = ConvertStringToInt(tmp);
    Net* n = new Net;
    for (int j = 0; j < net_degree; ++j) {
      string name;
      net_input >> name;
      assert(all_terminals.find(name) != all_terminals.end());
      Terminal* t = all_terminals.find(name)->second;
      n->terminals_.push_back(t);
    }
    all_nets.push_back(n);
  }

  srand(time(NULL));
  BTree floorplan(&all_macros, &all_nets);
  bool use_FSA = (atoi(argv[5]) == 1) ? true : false;
  if (use_FSA) {
    floorplan.FSA(alpha, outline_width, outline_height);
  } else {
    floorplan.SA(alpha, outline_width, outline_height);
  }
  floorplan.RecoverBestFloorplan();
  clock_t runtime = clock() - begin_time;

  floorplan.PackMacros();
  /* floorplan.PrintMacros(); */
  /* cout << floorplan.best_bounding_width_ << " " << floorplan.best_bounding_height_ << endl; */

  // Output
  int best_width = floorplan.best_bounding_width_;
  int best_height = floorplan.best_bounding_height_;
  int best_area = best_width * best_height;
  int best_wire_length = 0;
  for (size_t i = 0; i < all_nets.size(); ++i) {
    best_wire_length += all_nets[i]->HPWL();
  }
  double best_cost = alpha * best_area + (1 - alpha) * best_wire_length;

  output << best_cost << endl;
  output << best_wire_length << endl;
  output << best_area << endl;
  output << best_width << " " << best_height << endl;
  output << runtime / (double)CLOCKS_PER_SEC << endl;
  for (size_t i = 0; i < all_macros.size(); ++i) {
    output << all_macros[i]->name_ << " " << all_macros[i]->GetX() << " " << all_macros[i]->GetY() << " " << all_macros[i]->GetX() + all_macros[i]->width_ << " " << all_macros[i]->GetY() + all_macros[i]->height_ << endl;
  }

  for (size_t i = 0; i < all_macros.size(); ++i) {
    delete all_macros[i];
  }
  for (map<string, Terminal*>::iterator it = all_terminals.begin(); it != all_terminals.end(); ++it) {
    delete it->second;
  }
  for (size_t i = 0; i < all_nets.size(); ++i) {
    delete all_nets[i];
  }

  return 0;
}

int ConvertStringToInt(string str)
{
  int n;
  stringstream ss(str);
  ss >> n;
  return n;
}
