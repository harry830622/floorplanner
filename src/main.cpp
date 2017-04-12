#include "./floorplanner.hpp"

#include <fstream>
#include <cstdlib>

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

  Database database(block_input, net_input);
  /* database.Print(); */

  Floorplanner floorplanner(database, alpha);
  floorplanner.Run();

  ofstream output(argv[4]);
  floorplanner.Output(output);

  if (argc > 5) {
    ofstream json(argv[5]);
    floorplanner.Draw(json);
  }

  return 0;
}
