#ifndef STOPWATCH_H
#define STOPWATCH_H

namespace stw {

clock_t StartTime = -1;

void Start() {
  StartTime = clock();
}

double Stop() {
  clock_t Elapsed = 0;
  clock_t Stop = 0;

  if (StartTime > -1) {
    Stop = clock();
    Elapsed = Stop - StartTime;
    StartTime = -1;
  }

  return static_cast<double>(Elapsed) / static_cast<double>(CLOCKS_PER_SEC);
}

}  // namespace stw

#endif /* STOPWATCH_H */
