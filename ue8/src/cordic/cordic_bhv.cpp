/*****************************************************************************/
/**
 * @file    cordic_bhv.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Implementation of a behavioural model of the Cordic algorithm
 */
/*****************************************************************************/

#include "cordic_bhv.hpp"
#include <cmath>

using namespace std;

CordicBhv::CordicBhv(sc_module_name name) : sc_module(name)
{
  SC_THREAD(calc);
}

void CordicBhv::calc()
{
  oRdy.write(1);

  while (true)
  {
    wait(iStart.posedge_event());
    oRdy.write(0);
    wait(SC_ZERO_TIME);

    // cout << "## Start Cordic Calculation @ " << sc_time_stamp() << endl;

    phi_t phi = iPhi.read();
    double K = 1;
    double x = 1;
    double x_tmp = 1;
    double y = 0;
    double z = phi;
    int d = 1;

    for (int i = 0; i < ITERATIONS; i++)
    {
      if (z >= 0)
        d = 1;
      else
        d = -1;

      K = K * 1 / sqrt(1 + pow(2, -2 * i));
      z = z - d * atan(pow(2, -i));
      x_tmp = x - y * d * pow(2, -i);
      y = y + x * d * pow(2, -i);
      x = x_tmp;
    }

    oX.write(x * K);
    oY.write(y * K);

    oRdy.write(1);
  }
}
