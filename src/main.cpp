#include "../lib/helpers/helpers.hpp"
#include "../lib/uni-database/uni_database.hpp"
#include "../lib/uni-parser/uni_parser.hpp"

#include "floorplanner.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using helpers::Point;
using uni_parser::UniParser;
using uni_database::UniDatabase;
using uni_database::Keys;
using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cerr << "Usage: fp alpha input.block input.net output.rpt" << endl;
    return 1;
  }

  clock_t begin_time = clock();

  double alpha = stod(string(argv[1]));

  const int num_files = 2;
  ifstream file_streams[num_files];
  for (int i = 0; i < num_files; ++i) {
    file_streams[i].open(argv[i + 2]);
    if (!file_streams[i].is_open()) {
      cerr << "Cannot open file: " << argv[i + 2] << endl;
      return 2;
    }
  }

  UniDatabase database;
  UniParser parser;

  ifstream blocks_input(argv[2]);
  int num_macros = 0;
  int num_terminals = 0;
  parser.Parse(
      blocks_input,
      [&](const vector<string>& tokens) {
        if (!tokens.empty()) {
          string keyword = tokens.at(0);
          if (keyword == "Outline") {
            int outline_width = stoi(tokens.at(1));
            int outline_height = stoi(tokens.at(2));
            database.Set(Keys{"outline", "width"}, outline_width);
            database.Set(Keys{"outline", "height"}, outline_height);
          } else if (keyword == "NumBlocks") {
            num_macros = stoi(tokens.at(1));
          } else if (keyword == "NumTerminals") {
            num_terminals = stoi(tokens.at(1));
          } else {
            if (num_macros != 0) {
              string macro_name = tokens.at(0);
              int macro_width = stoi(tokens.at(1));
              int macro_height = stoi(tokens.at(2));
              database.Set(Keys{"macros", macro_name, "name"}, macro_name);
              database.Set(Keys{"macros", macro_name, "width"}, macro_width);
              database.Set(Keys{"macros", macro_name, "height"}, macro_height);
              --num_macros;
            } else if (num_terminals != 0) {
              string terminal_name = tokens.at(0);
              int terminal_x = stoi(tokens.at(2));
              int terminal_y = stoi(tokens.at(3));
              database.Set(Keys{"terminals", terminal_name, "name"},
                           terminal_name);
              database.Set(Keys{"terminals", terminal_name, "x"}, terminal_x);
              database.Set(Keys{"terminals", terminal_name, "y"}, terminal_y);
              --num_terminals;
            }
          }
        }
      },
      string(":"));

  ifstream nets_input(argv[3]);
  parser.Parse(
      nets_input,
      [&](const vector<string>& tokens) {
        if (!tokens.empty()) {
          string keyword = tokens.at(0);
          if (keyword == "NumNets") {
            int num_nets = stoi(tokens.at(1));
            auto begin_handler = [](const vector<string>& tokens) {};
            int i = 0;
            auto handler = [&](const vector<string>& tokens) {
              string keyword = tokens.at(0);
              if (keyword == "NetDegree") {
                int net_degree = stoi(tokens.at(1));
                auto begin_handler = [](const vector<string>& tokens) {};
                auto handler = [&](const vector<string>& tokens) {
                  if (database.Id(Keys{"macros", tokens.at(0)}) != -1) {
                    database.Set({"nets", string("N") + to_string(i), "macros",
                                  tokens.at(0)},
                                 database.Id(Keys{"macros", tokens.at(0)}));
                  } else if (database.Id(Keys{"terminals", tokens.at(0)}) !=
                             -1) {
                    database.Set({"nets", string("N") + to_string(i),
                                  "terminals", tokens.at(0)},
                                 database.Id(Keys{"terminals", tokens.at(0)}));
                  }
                };
                auto end_handler = []() {};
                parser.Parse(nets_input, tokens, keyword, net_degree,
                             begin_handler, handler, end_handler, string(":"));
                ++i;
              };
            };
            auto end_handler = []() {};
            parser.Parse(nets_input, tokens, keyword, num_nets, begin_handler,
                         handler, end_handler, string(":"));
          }
        }
      },
      string(":"));

  Floorplanner floorplanner(database, alpha);

  srand(time(0));
  floorplanner.Run();

  double run_time =
      (clock() - begin_time) / static_cast<double>(CLOCKS_PER_SEC);

  ofstream output_file(argv[4]);
  const Floorplan& best_floorplan = floorplanner.best_floorplan();
  int chip_width = best_floorplan.width();
  int child_height = best_floorplan.height();
  int chip_area = chip_width * child_height;
  double wire_length = best_floorplan.WireLength(database);
  output_file << alpha * chip_area + (1 - alpha) * wire_length << endl;
  output_file << wire_length << endl;
  output_file << chip_area << endl;
  output_file << chip_width << " " << child_height << endl;
  output_file << run_time << endl;
  best_floorplan.IterateMacros(
      [&](int data_id, const Point<int>& coordinate, bool is_rotated) {
        string name = database.Data<string>(data_id, "name");
        int x = coordinate.x();
        int y = coordinate.y();
        int width = database.Data<int>(data_id, "width");
        int height = database.Data<int>(data_id, "height");
        if (is_rotated) {
          swap(width, height);
        }
        output_file << name << " " << x << " " << y << " " << x + width << " "
                    << y + height << endl;
      });

  return 0;
}
