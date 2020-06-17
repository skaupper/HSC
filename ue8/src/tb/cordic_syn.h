#ifndef _CORDIC_SYN_H_
#define _CORDIC_SYN_H_

#ifdef __RTL_SIMULATION__
#include "cordic.hpp"
#include "cordic_cc_rtl_wrapper.h"
#define cordic_cc cordic_cc_rtl_wrapper
#else
#include "cordic/cordic_cc.hpp"
#endif

#endif
