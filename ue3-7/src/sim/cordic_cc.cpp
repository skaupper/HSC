/*****************************************************************************/
/**
 * @file    cordic.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Implementation of a cycle accurate Cordic algorithm
 */
/*****************************************************************************/

#include "cordic_cc.hpp"
#include <cmath>

using namespace std;

static double customAtan(int i)
{
  static double LUT[ITERATIONS];
  static bool lutInitialized = false;

  if (!lutInitialized) {
    for(int i = 0; i < ITERATIONS; ++i) {
      LUT[i] = atan(pow(2, -i));
    }

    lutInitialized = true;
  }

  return LUT[i];
}

static double precalcK()
{
  // Calculates the gain K using the formula K = K / sqrt(1 + pow(2, -2 * i))
  // For K_0=1 and ITERATIONS iterations

  static double K = 1;
  static bool initialized = false;

  if (!initialized) {
    for (int i = 0; i < ITERATIONS; ++i) {
      K = K / sqrt(1 + pow(2, -2*i));
    }

    initialized = true;
  }

  return K;
}


CordicCc::CordicCc(sc_module_name name) : sc_module(name)
{
  SC_CTHREAD(calc, iClk.pos());
  reset_signal_is(inRst, false);
}

void CordicCc::calc()
{
  //
  // Reset routines
  //
  oRdy.write(1);

  wait();


  //
  // Variables used
  //

  // Control state
  bool calcStarted = false;
  int currentIter = 0;

  // Constants
  static const double K = precalcK();

  // Intermediate values
  phi_t phi;
  double x;
  double x_tmp;
  double y;
  double z;
  int d;


  while (true)
  {
    // If no calculation is started and iStart is low, do nothing.
    if (iStart == 0 && !calcStarted) {
      // cout << "Wait for iStart... @" << sc_time_stamp() << endl;
      wait();
      continue;
    }

    // Start a calculation
    if (iStart == 1 && !calcStarted) {
      currentIter = 0;

      // Initial values
      phi   = iPhi.read();
      x     = 1;
      x_tmp = 1;
      y     = 0;
      z     = phi;
      d     = 1;

      // Deassert oRdy until the calculation finished
      oRdy.write(0);
      calcStarted = true;
    }

    // cout << "## Start Cordic Calculation Iteration " << currentIter << " @ " << sc_time_stamp() << endl;

    //
    // Do calculation
    //

    if (z >= 0) {
      d = 1;
    } else {
      d = -1;
    }

    z = z - d * customAtan(currentIter);
    x_tmp = x - y * d * pow(2, -currentIter);
    y = y + x * d * pow(2, -currentIter);
    x = x_tmp;



    // Check if the calculation ended
    if (currentIter == ITERATIONS-1) {
      oX.write(x * K);
      oY.write(y * K);
      oRdy.write(1);
      calcStarted = false;
    }

    currentIter++;

    wait();
  }
}
