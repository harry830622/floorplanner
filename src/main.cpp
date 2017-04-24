#include "./floorplanner.hpp"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <ctime>
#include <fstream>

using namespace std;
using Json = nlohmann::json;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
  po::options_description options("Options");

  // clang-format off
  options.add_options()
    ("help,h", "Print help messages")
    ("seed", po::value<long>(), "Seed for random number generator")
    ("sa", po::bool_switch(), "Use classical SA")
    ("draw", po::value<string>(), "Draw floorplans")
    ("draw-only-best", po::value<string>(), "Draw only best floorplan")
    ("alpha", po::value<double>()->required(), "Alpha")
    ("block-input", po::value<string>()->required(), "Block input")
    ("net-input", po::value<string>()->required(), "Net input")
    ("output", po::value<string>()->required(), "Output");
  // clang-format on

  po::positional_options_description positional_options;
  positional_options.add("alpha", 1);
  positional_options.add("block-input", 1);
  positional_options.add("net-input", 1);
  positional_options.add("output", 1);

  po::variables_map arguments;

  po::store(po::command_line_parser(argc, argv)
                .options(options)
                .positional(positional_options)
                .run(),
            arguments);

  if (arguments.count("help") || argc < 5) {
    cout << "Usage:" << endl;
    cout
        << "  " << argv[0]
        << " [--seed <number>] [--sa] [--draw <file>] [--draw-only-best <file>]"
        << " <alpha> <block-input> <net-input> <output>" << endl;
    cout << options;
    return 0;
  }

  const double alpha = arguments["alpha"].as<double>();
  if (alpha < 0 || alpha > 1) {
    cout << "Error: alpha should range between 0 and 1" << endl;
    return 1;
  }

  const long seed =
      arguments.count("seed") == 1 ? arguments["seed"].as<long>() : time(NULL);
  srand(seed);

  ifstream block_input(arguments["block-input"].as<string>());
  ifstream net_input(arguments["net-input"].as<string>());

  clock_t time_begin = clock();

  Database database(block_input, net_input);
  /* database.Print(); */

  const bool is_using_fast_sa = arguments.count("sa") == 1 ? false : true;

  const bool is_drawing = arguments.count("draw") == 1 ? true : false;

  Floorplanner floorplanner(database, alpha, is_using_fast_sa, is_drawing);
  floorplanner.Run();

  double runtime = (clock() - time_begin) / static_cast<double>(CLOCKS_PER_SEC);

  const Floorplan& best_floorplan = floorplanner.best_floorplan();
  const double outline_width = database.outline_width();
  const double outline_height = database.outline_height();
  const double best_width = best_floorplan.width();
  const double best_height = best_floorplan.height();
  const double best_area = best_floorplan.area();
  const double best_wirelength = best_floorplan.wirelength();
  const double best_cost = alpha * best_area + (1 - alpha) * best_wirelength;

  cout << endl;
  cout << "============================= SUMMARY =============================="
       << endl;
  cout << "outline width:\t" << outline_width << "\t\toutline height:\t\t"
       << outline_height << endl;
  cout << "best width:\t" << best_width << "\t\tbest height:\t\t" << best_height
       << endl;
  cout << "best area:\t" << best_area << "\tbest wirelength:\t"
       << best_wirelength << endl;
  cout << "alpha:\t\t" << alpha << "\t\tbest cost:\t\t" << best_cost << endl;
  cout << "runtime:\t" << runtime << endl;

  ofstream output(arguments["output"].as<string>());
  output << best_cost << endl;
  output << best_wirelength << endl;
  output << best_area << endl;
  output << best_width << " " << best_height << endl;
  output << runtime << endl;
  for (int i = 0; i < database.num_macros(); ++i) {
    const int macro_id = i;
    const string macro_name = database.macro(macro_id).name();
    auto bounding_box = best_floorplan.macro_bounding_box(macro_id);
    const Point& lower_left = bounding_box.first;
    const Point& upper_right = bounding_box.second;
    output << macro_name << " " << lower_left.x() << " " << lower_left.y()
           << " " << upper_right.x() << " " << upper_right.y() << endl;
  }

  if (is_drawing) {
    ofstream drawing_output(arguments["draw"].as<string>());
    Json drawing = floorplanner.drawing();
    fs::path block_input_path(arguments["block-input"].as<string>());
    drawing["benchmark"] = block_input_path.stem().string();
    drawing["alpha"] = alpha;
    drawing["outline"] = {{"width", database.outline_width()},
                          {"height", database.outline_height()}};
    drawing["seed"] = seed;
    drawing["best_floorplan"] = best_floorplan.drawing();
    drawing["runtime"] = runtime;
    drawing_output << drawing;
  }

  const bool is_drawing_only_best =
      arguments.count("draw-only-best") == 1 ? true : false;

  if (is_drawing_only_best) {
    ofstream drawing_output(arguments["draw-only-best"].as<string>());
    Json drawing = Json::object();
    fs::path block_input_path(arguments["block-input"].as<string>());
    drawing["benchmark"] = block_input_path.stem().string();
    drawing["alpha"] = alpha;
    drawing["outline"] = {{"width", database.outline_width()},
                          {"height", database.outline_height()}};
    drawing["seed"] = seed;
    drawing["best_floorplan"] = Json::object();
    Json& best_floorplan_drawing = drawing["best_floorplan"];
    best_floorplan_drawing["macros"] = Json::array();
    for (int i = 0; i < database.num_macros(); ++i) {
      const int macro_id = i;
      const Macro& macro = database.macro(macro_id);
      const auto& macro_bounding_box =
          best_floorplan.macro_bounding_box(macro_id);
      const Point& macro_lower_left = macro_bounding_box.first;
      const Point& macro_upper_right = macro_bounding_box.second;
      const string& macro_name = macro.name();
      best_floorplan_drawing["macros"].push_back(
          {{"name", macro_name},
           {"lower_left",
            {{"x", macro_lower_left.x()}, {"y", macro_lower_left.y()}}},
           {"upper_right",
            {{"x", macro_upper_right.x()}, {"y", macro_upper_right.y()}}}});
    }
    best_floorplan_drawing["width"] = best_floorplan.width();
    best_floorplan_drawing["height"] = best_floorplan.height();
    best_floorplan_drawing["area"] = best_floorplan.area();
    best_floorplan_drawing["wirelength"] = best_floorplan.wirelength();
    drawing["runtime"] = runtime;
    drawing_output << drawing;
  }

  return 0;
}
