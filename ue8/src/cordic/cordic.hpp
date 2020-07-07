/*****************************************************************************/
/**
 * @file    cordic.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Header of Cordic algorithm
 */
/*****************************************************************************/

#ifndef _CORDIC_HPP_
#define _CORDIC_HPP_

#ifndef SC_INCLUDE_FX
#define SC_INCLUDE_FX  // for fixed-point data types (like sc_ufixed)
#endif

#include <systemc.h>

// 18 iterations to use the maximum resolution of the interface (max. error =
// 2^-16) static const int ITERATIONS = 18;
const uint8_t ITERATIONS = 16;

const uint8_t PHI_FRAC_BITS = 21;
const uint8_t PHI_INT_BITS = 1;
const uint8_t PHI_WL = (PHI_INT_BITS + PHI_FRAC_BITS);
const uint8_t PHI_IWL = (PHI_INT_BITS);

const uint8_t XY_FRAC_BITS = 16;
const uint8_t XY_INT_BITS = 0;
const uint8_t XY_WL = (XY_INT_BITS + XY_FRAC_BITS);
const uint8_t XY_IWL = (XY_INT_BITS);

/* NOTE: The format for ufixed is: ufixed<TOTAL_BITS, INTEGER_BITS>  */
typedef sc_ufixed<PHI_WL, PHI_IWL, SC_RND, SC_SAT> phi_t;
typedef sc_ufixed<XY_WL, XY_IWL, SC_RND, SC_SAT> xy_t;

#endif /* _CORDIC_HPP_ */
