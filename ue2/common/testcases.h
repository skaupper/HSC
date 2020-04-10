#ifndef _TESTCASES_H_
#define _TESTCASES_H_

#include "StopWatch.h"
#include "parameters.h"

/* Main test sequence */
template <typename T>
void run_test_sequence(T &master) {
  uint32_t wr_data;
  uint32_t rd_data;
  srand(time(nullptr));

  std::cout << "### Perform write on all addresses ###" << std::endl;
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    wr_data = addr * 3;
    master.singleWrite(addr, wr_data);
  }
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;

  std::cout << "### Perform read on all addresses ###" << std::endl;
  stw::Start();
  for (uint32_t addr = 0; addr < MEMORY_DEPTH; addr++) {
    rd_data = master.singleRead(addr);

    sc_assert(rd_data == addr * 3);
  }
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;

  std::cout << "### Perform " << NUM_RANDOM_TESTS
            << " write-read-checks on random addresses ###" << std::endl;
  stw::Start();
  uint32_t addr;
  for (uint32_t i = 0; i < NUM_RANDOM_TESTS; i++) {
    wr_data = rand();
    addr = rand() % MEMORY_DEPTH;

    master.singleWrite(addr, wr_data);
    rd_data = master.singleRead(addr);

    sc_assert(rd_data == wr_data);
  }
  std::cout << "Took " << stw::Stop() << " seconds.\n" << std::endl;

  std::cout << "### Test sequence done [" << sc_time_stamp() << "] ###"
            << std::endl;
}

#endif /* _TESTCASES_H */
