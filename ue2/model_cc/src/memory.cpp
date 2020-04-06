#include "memory.h"

using namespace std;

Memory::Memory(sc_module_name name) : sc_module(name) {
  SC_THREAD(calc);
}

void Memory::calc() {
}
