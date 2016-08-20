#include "../lib/helpers/helpers.hpp"
#include "../lib/uni-database/uni_database.hpp"
#include "../lib/uni-parser/uni_parser.hpp"

#include <boost/any.hpp>
#include <fstream>
#include <iostream>

using helpers::ConvertStringTo;
using uni_parser::UniParser;
using uni_database::UniDatabase;
using boost::any_cast;
using namespace std;

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

  UniDatabase datebase;
  UniParser parser;

  ifstream blocks_input(argv[2]);
  int num_macros = 0;
  int num_terminals = 0;
  parser.Parse(
      blocks_input,
      [&](const vector<string>& tokens) {
        if (!tokens.empty()) {
          string first_word = tokens.at(0);
          if (first_word == "Outline") {
            int outline_width = ConvertStringTo<int>(tokens.at(1));
            int outline_height = ConvertStringTo<int>(tokens.at(2));
            datebase.Set({"outline", "width"}, outline_width);
            datebase.Set({"outline", "height"}, outline_height);
          } else if (first_word == "NumBlocks") {
            num_macros = ConvertStringTo<int>(tokens.at(1));
          } else if (first_word == "NumTerminals") {
            num_terminals = ConvertStringTo<int>(tokens.at(1));
          } else {
            if (num_macros != 0) {
              string macro_name = tokens.at(0);
              int macro_width = ConvertStringTo<int>(tokens.at(1));
              int macro_height = ConvertStringTo<int>(tokens.at(2));
              datebase.Set({"macros", macro_name, "width"}, macro_width);
              datebase.Set({"macros", macro_name, "height"}, macro_height);
              --num_macros;
            } else if (num_terminals != 0) {
              string terminal_name = tokens.at(0);
              int terminal_x = ConvertStringTo<int>(tokens.at(2));
              int terminal_y = ConvertStringTo<int>(tokens.at(3));
              datebase.Set({"terminals", terminal_name, "x"}, terminal_x);
              datebase.Set({"terminals", terminal_name, "y"}, terminal_y);
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
          string first_word = tokens.at(0);
          if (first_word == "NumNets") {
            int num_nets = ConvertStringTo<int>(tokens.at(1));
            auto begin_handler = [](const vector<string>& tokens) {};
            auto handler = [&](const vector<string>& tokens) {
              string first_word = tokens.at(0);
              if (first_word == "NetDegree") {
                int net_degree = ConvertStringTo<int>(tokens.at(1));
                auto begin_handler = [](const vector<string>& tokens) {};
                int i = 0;
                auto handler = [&](const vector<string>& tokens) {
                  datebase.Set({"nets", string("N") + to_string(i++),
                                "terminals", tokens.at(0)},
                               'X');
                };
                auto end_handler = []() {};
                parser.Parse(nets_input, tokens, first_word, net_degree,
                             begin_handler, handler, end_handler, string(":"));
              };
            };
            auto end_handler = []() {};
            parser.Parse(nets_input, tokens, first_word, num_nets,
                         begin_handler, handler, end_handler, string(":"));
          }
        }
      },
      string(":"));

  // TODO:

  clock_t run_time = clock() - begin_time;

  ofstream output_file(argv[4]);

  return 0;
}
