#include "../lib/helpers/helpers.hpp"
#include "../lib/uni-parser/uni_parser.hpp"
#include "database.hpp"
#include "floorplanner.hpp"

#include <fstream>
#include <iostream>

using namespace std;
using helpers::ConvertStringTo;
using uni_parser::Parse;

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cerr << "Usage: fp alpha input.block input.net output.rpt" << endl;
    return 1;
  }

  clock_t begin_time = clock();

  float alpha = ConvertStringTo<float>(string(argv[1]));

  const int num_files = 2;
  ifstream file_streams[num_files];
  for (int i = 0; i < num_files; ++i) {
    file_streams[i].open(argv[i + 2]);
    if (!file_streams[i].is_open()) {
      cerr << "Cannot open file: " << argv[i + 2] << endl;
      return 2;
    }
  }

  Database database;

  ifstream blocks_input(argv[2]);
  int num_blocks = 0;
  int num_terminals = 0;
  Parse(blocks_input,
        [&database, &num_blocks, &num_terminals](const vector<string>& tokens) {
          if (!tokens.empty()) {
            string first_word = tokens.at(0);
            if (first_word == "Outline") {
              database.SetOutlineWidth(ConvertStringTo<int>(tokens.at(1)));
              database.SetOutlineHeight(ConvertStringTo<int>(tokens.at(2)));
            } else if (first_word == "NumBlocks") {
              num_blocks = ConvertStringTo<int>(tokens.at(1));
            } else if (first_word == "NumTerminals") {
              num_terminals = ConvertStringTo<int>(tokens.at(1));
            } else {
              if (num_blocks != 0) {
                database.AddNewMacro(tokens.at(0),
                                     ConvertStringTo<int>(tokens.at(1)),
                                     ConvertStringTo<int>(tokens.at(2)));
                --num_blocks;
              } else if (num_terminals != 0) {
                database.AddNewTerminal(tokens.at(0),
                                        ConvertStringTo<int>(tokens.at(2)),
                                        ConvertStringTo<int>(tokens.at(3)));
                --num_terminals;
              }
            }
          }
        },
        string(" :\t\r"));

  ifstream nets_input(argv[3]);
  int num_nets = 0;
  int net_degree = 0;
  int net_idx;
  Parse(nets_input,
        [&database, &num_nets, &net_degree,
         &net_idx](const vector<string>& tokens) {
          if (!tokens.empty()) {
            string first_word = tokens.at(0);
            if (first_word == "NumNets") {
              num_nets = ConvertStringTo<int>(tokens.at(1));
            } else if (first_word == "NetDegree") {
              net_degree = ConvertStringTo<int>(tokens.at(1));
              net_idx = database.AddNewNet();
            } else {
              if (num_nets != 0) {
                if (net_degree != 0) {
                  int terminal_idx = database.GetTerminalIdx(tokens.at(0));
                  database.AddNetTerminal(net_idx, terminal_idx);
                  --net_degree;
                  if (net_degree == 0) {
                    --num_nets;
                  }
                }
              }
            }
          }
        },
        string(" :\t\r"));

  Floorplanner floorplanner(database, alpha);

  srand(time(0));
  floorplanner.Run();

  clock_t run_time = clock() - begin_time;

  ofstream output_file(argv[4]);
  const Floorplan& floorplan = floorplanner.GetBestFloorPlan();
  int width = floorplan.GetWidth();
  int height = floorplan.GetHeight();
  int area = width * height;
  float wire_length = floorplanner.CalculateBestWireLength();
  output_file << alpha * area + (1 - alpha) * wire_length << endl;
  output_file << wire_length << endl;
  output_file << floorplan.GetWidth() * floorplan.GetHeight() << endl;
  output_file << floorplan.GetWidth() << " " << floorplan.GetHeight() << endl;
  output_file << run_time / static_cast<float>(CLOCKS_PER_SEC) << endl;
  for (int i = 0; i < floorplan.GetNumMacroInstances(); ++i) {
    int x = floorplan.GetMacroInstanceX(i);
    int y = floorplan.GetMacroInstanceY(i);
    bool is_rotated = floorplan.GetMacroInstanceIsRotated(i);
    int macro_idx = floorplan.GetMacroInstanceMacroIdx(i);
    string name = database.GetMacroName(macro_idx);
    int width = database.GetMacroWidth(macro_idx);
    int height = database.GetMacroHeight(macro_idx);
    if (is_rotated) {
      swap(width, height);
    }
    output_file << name << " " << x << " " << y << " " << x + width << " "
                << y + height << endl;
  }

  return 0;
}
