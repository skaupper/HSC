#include "CordicTb.h"
#include <cmath>

bool check_equal(const double d1, const double d2, const double epsilon)
{
  return std::abs(d1 - d2) < epsilon;
}

void print_results(const double exp_cos, const double act_cos,
                   const double exp_sin, const double act_sin)
{
  std::cout << "Expected cosinus: " << exp_cos << "; Actual cosinus: " << act_cos << std::endl;
  std::cout << "Expected sinus: " << exp_sin << "; Actual sinus: " << act_sin << std::endl;
}

void CordicTb::run_test(const double phi)
{
  static const double EPSILON = 0.001;

  double exp_cos, exp_sin; // Variables which hold the expected values
  double act_cos, act_sin; // Variables which hold the actual values

  exp_cos = cos(phi);
  exp_sin = sin(phi);

  oPhi = phi;
  oStart = true;
  wait(iRdy.posedge_event());
  oStart = false;
  wait(0, SC_NS);

  act_cos = iX.read().to_double();
  act_sin = iY.read().to_double();

  if (!check_equal(act_cos, exp_cos, EPSILON) ||
      !check_equal(act_sin, exp_sin, EPSILON))
  {
    std::cerr << "CORDIC algorithm failed!" << std::endl;
    print_results(exp_cos, act_cos, exp_sin, act_sin);
  }
}

void CordicTb::doStimulate()
{
  static const double INCR = 0.01;

  for (double phi = 0.0; phi <= M_PI / 2; phi += INCR)
  {
    run_test(0.0);
  }
}
