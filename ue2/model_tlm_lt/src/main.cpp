#include "top.h"

int sc_main(int argc, char* argv[]) {
  top top("top");
  sc_start();

  cout << "Simulation took " << sc_delta_count() << " delta cycles." << endl;

  return 0;
}
