#ifndef _TESTCASES_H_
#define _TESTCASES_H_

/* Output debug messages */
#define OUTPUT_DEBUG_MSG false

/* Number of randomized write-read tests that are executed */
#define NUM_RANDOM_TESTS (uint32_t)(pow(10, 2))

/* Number of memory elements in the memory slave */
#define MEMORY_DEPTH 4096

/* Clock period in nanoseconds */
#define CLK_PERIOD_NS 10

#endif /* _TESTCASES_H */
