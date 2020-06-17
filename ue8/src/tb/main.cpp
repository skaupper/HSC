#include "top.h"

int sc_main(int argc, char *argv[]) {
  Top top("top");

  sc_start();

  if (top.mSuccess.read()) {
    return 0;
  } else {
    return 1;
  }
}
