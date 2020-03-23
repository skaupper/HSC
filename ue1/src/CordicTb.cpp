#include "CordicTb.h"
#include <cmath>

bool check_equal(const double d1, const double d2, const double epsilon)
{
  return std::abs(d1 - d2) < epsilon;
}

bool CordicTb::run_test(const double phi)
{
  /* Theory: Max. EPSILON = 2^-ITERATIONS
   * Reality: EPSILON = 2^-ITERATIONS + TRUNCATION ERROR (iPhi and oX, oY)
   */
  static const double EPSILON = std::pow(2, -14);

  double exp_cos, exp_sin; // Variables which hold the expected values
  double act_cos, act_sin; // Variables which hold the actual values

  // Expected outputs
  exp_cos = cos(phi);
  exp_sin = sin(phi);

  // Start calculation and wait for a result
  oPhi = phi;
  oStart = true;
  wait(iRdy.posedge_event());
  oStart = false;
  wait(0, SC_NS);

  // Get the result and compare against the expected values
  act_cos = iX.read().to_double();
  act_sin = iY.read().to_double();

  if (!check_equal(act_cos, exp_cos, EPSILON) ||
      !check_equal(act_sin, exp_sin, EPSILON))
  {
    std::cerr << "CORDIC algorithm failed!" << std::endl;
    std::cout << "Phi: " << phi << std::endl;
    std::cout << "Expected cosinus: " << exp_cos << "; Actual cosinus: " << act_cos << std::endl;
    std::cout << "Expected sinus: " << exp_sin << "; Actual sinus: " << act_sin << std::endl;
    return false;
  }

  return true;
}

void CordicTb::doStimulate()
{
  static const double INCR = 0.001;

  // Test the CORDIC implementation with a lot values between 0° and 90°
  for (double phi = 0.0; phi <= M_PI / 2; phi += INCR)
  {
    bool success = run_test(phi);
    if (!success)
    {
      break;
    }
  }
}
