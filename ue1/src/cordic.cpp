#include "cordic.h"

using namespace std;

Cordic::Cordic(sc_module_name name) : sc_module(name)
{
  SC_THREAD(calc);
}

void Cordic::calc()
{
  while (true)
  {
    oRdy.write(0);
    wait(iStart.posedge_event());
    cout << "## Start Cordic Calculation @ " << sc_time_stamp() << endl;

    phi_t phi = iPhi.read();

    oX.write(cos(phi.to_double()));
    oY.write(sin(phi.to_double()));

    oRdy.write(1);
    wait(0, SC_NS);
  }
}

// void Cordic::calc()
// {
//   while (true)
//   {
//     oRdy.write(0);
//     wait(iStart.posedge_event());
//     cout << "## Start Cordic Calculation @ " << sc_time_stamp() << endl;
//
//     phi_t phi = iPhi.read();
//
//     oX.write(cos(phi.to_double()));
//     oY.write(sin(phi.to_double()));
//
//     oRdy.write(1);
//     wait(0, SC_NS);
//   }
// }
