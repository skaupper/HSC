#include "CordicTb.h"

#include <cmath>

bool check_equal(const double d1, const double d2, const double epsilon) {
  return std::abs(d1 - d2) < epsilon;
}

bool CordicTb::run_test(const double phi, double *pError) {
  /* Theory: Max. EPSILON = 2^-(ITERATIONS-1)
   * Reality: EPSILON = 2^-(ITERATIONS-1) + TRUNCATION ERROR (iPhi and oX, oY)
   */
  // With 18 iterations, EPSILON can be increased to 2^-16
  // static const double EPSILON = std::pow(2, -16);
  static const double EPSILON = std::pow(2, -14);

  double exp_cos, exp_sin;  // Variables which hold the expected values
  double act_cos, act_sin;  // Variables which hold the actual values

  // Expected outputs
  exp_cos = cos(phi);
  exp_sin = sin(phi);

  // iRdy may already be true
  if (iRdy == false) {
    wait(iRdy.posedge_event());
  }

  // Start calculation and wait for a result
  oPhi = phi;
  oStart = true;
  wait(iRdy.negedge_event());
  oStart = false;
  wait(iRdy.posedge_event());

  // Get the result and compare against the expected values
  act_cos = iX.read().to_double();
  act_sin = iY.read().to_double();

  // Calculate the maximum error of the current test case
  if (pError) {
    double errorCos, errorSin;
    errorCos = std::abs(exp_cos - act_cos);
    errorSin = std::abs(exp_sin - act_sin);

    if (errorCos > errorSin) {
      *pError = errorCos;
    } else {
      *pError = errorSin;
    }
  }

  if (!check_equal(act_cos, exp_cos, EPSILON) ||
      !check_equal(act_sin, exp_sin, EPSILON)) {
    std::cerr << "CORDIC algorithm failed!" << std::endl;
    std::cout << "Phi: " << phi << std::endl;
    std::cout << "Expected cosinus: " << exp_cos
              << "; Actual cosinus: " << act_cos << std::endl;
    std::cout << "Expected sinus: " << exp_sin << "; Actual sinus: " << act_sin
              << std::endl;
    return false;
  }

  return true;
}

void CordicTb::doStimulate() {
  static const double INCR = 0.001;
  bool success = true;
  double error, maxError = 0;

  wait(inRst.posedge_event());

  // Test the CORDIC implementation with a lot values between 0° and 90°
  for (double phi = 0.0; phi <= M_PI / 2; phi += INCR) {
    if (!run_test(phi, &error))
      success = false;

    if (error > maxError)
      maxError = error;
  }

  std::cout << "Max. error: " << maxError << " ("
            << std::abs(std::log2(maxError)) << " bits)" << std::endl;

  if (!success) {
    std::cerr << "Simulation finished with error!" << std::endl;
  } else {
    std::cout << "Simulation finished successfully!" << std::endl;
  }

  oSuccess = success;

  sc_stop();
}
