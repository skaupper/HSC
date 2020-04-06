#include "master.h"

bool check_equal(const double d1, const double d2, const double epsilon) {
  return std::abs(d1 - d2) < epsilon;
}

void Master::doStimulate() {
}
