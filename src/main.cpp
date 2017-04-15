#include "./floorplanner.hpp"

#include <ctime>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 5) {
    cout << "Usage: " << argv[0]
         << " [alpha] [block_input] [net_input] [output]" << endl;
    return 1;
  }

  const double alpha = stod(string(argv[1]));
  if (alpha < 0 || alpha > 1) {
    cout << "Error: alpha should range between 0 and 1!" << endl;
    return 2;
  }

  srand(time(NULL));

  ifstream block_input(argv[2]);
  ifstream net_input(argv[3]);

  clock_t time_begin = clock();

  Database database(block_input, net_input);
  /* database.Print(); */

  Floorplanner floorplanner(database, alpha);
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

  ofstream output(argv[4]);
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

  if (argc > 5) {
    ofstream json(argv[5]);
    floorplanner.Draw(json);
  }

  return 0;
}
