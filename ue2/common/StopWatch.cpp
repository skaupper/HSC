#include "StopWatch.h"

#include <ctime>

clock_t StartTime = -1;

void stw::Start() {
  StartTime = clock();
}

double stw::Stop() {
  clock_t Elapsed = 0;
  clock_t Stop = 0;

  if (StartTime > -1) {
    Stop = clock();
    Elapsed = Stop - StartTime;
    StartTime = -1;
  }

  return static_cast<double>(Elapsed) / static_cast<double>(CLOCKS_PER_SEC);
}
