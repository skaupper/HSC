#ifndef _TESTCASES_H_
#define _TESTCASES_H_

#include "StopWatch.h"
#include "parameters.h"

template <typename T>
static void waitWithOptionalDebugOutput(T &master) {
#if (ACCUMULATED_WAIT == true)
  cout << "sc_time_stamp before: " << sc_time_stamp().to_seconds() << endl;
  cout << "waiting accumulated delay (" << master.mAccumulatedDelay.to_seconds()
       << " s)" << endl;
  wait(master.mAccumulatedDelay);
  cout << "sc_time_stamp after: " << sc_time_stamp().to_seconds() << endl;
#endif
}

template <typename T>
static void write_all_sequence(T &master) {
  uint32_t wr_data;
  std::cout << "### Perform write on all addresses ###" << std::endl;
#if (ACCUMULATED_WAIT == true)
  master.mAccumulatedDelay = SC_ZERO_TIME;
#endif
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    wr_data = addr * 3;
    master.singleWrite(addr, wr_data);
  }
  waitWithOptionalDebugOutput(master);
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;
}

template <typename T>
static void read_all_sequence(T &master) {
  uint32_t rd_data;
  std::cout << "### Perform read on all addresses ###" << std::endl;
#if (ACCUMULATED_WAIT == true)
  master.mAccumulatedDelay = SC_ZERO_TIME;
#endif
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    rd_data = master.singleRead(addr);

    sc_assert(rd_data == addr * 3);
  }
  waitWithOptionalDebugOutput(master);
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;
}

template <typename T>
static void write_read_random_sequence(T &master) {
  uint32_t wr_data;
  uint32_t rd_data;
  std::cout << "### Perform " << NUM_RANDOM_TESTS
            << " write-read-checks on random addresses ###" << std::endl;
#if (ACCUMULATED_WAIT == true)
  master.mAccumulatedDelay = SC_ZERO_TIME;
#endif
  stw::Start();
  uint32_t addr;
  for (uint32_t i = 0; i < NUM_RANDOM_TESTS; i++) {
    wr_data = rand();
    addr = rand() % MEMORY_DEPTH;

    master.singleWrite(addr, wr_data);
    rd_data = master.singleRead(addr);

    sc_assert(rd_data == wr_data);
  }
  waitWithOptionalDebugOutput(master);
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;
}

/* Main test sequence */
template <typename T>
void run_test_sequences(T &master) {
  srand(time(nullptr));

  write_all_sequence(master);

  read_all_sequence(master);

  write_read_random_sequence(master);

  std::cout << "### Test sequence done [ @ simulation time "
            << sc_time_stamp().to_seconds() << " s] ###" << std::endl;
  cout << "Simulation took " << sc_delta_count() << " delta cycles." << endl;
}

#endif /* _TESTCASES_H */
