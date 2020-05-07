/*****************************************************************************/
/**
 * @file    cordic.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Header of Cordic algorithm
 */
/*****************************************************************************/

#ifndef _CORDIC_H
#define _CORDIC_H

#define SC_INCLUDE_FX // for fixed-point data types (like sc_ufixed)
#include <systemc.h>


constexpr uint8_t PHI_FRAC_BITS = 21;
constexpr uint8_t PHI_INT_BITS = 1;
constexpr uint8_t PHI_WL = (PHI_INT_BITS + PHI_FRAC_BITS);
constexpr uint8_t PHI_IWL = (PHI_INT_BITS);

constexpr uint8_t XY_FRAC_BITS = 16;
constexpr uint8_t XY_INT_BITS = 0;
constexpr uint8_t XY_WL = (XY_INT_BITS + XY_FRAC_BITS);
constexpr uint8_t XY_IWL = (XY_INT_BITS);


/* NOTE: The format for ufixed is: ufixed<TOTAL_BITS, INTEGER_BITS>  */
typedef sc_ufixed<PHI_WL, PHI_IWL, SC_RND, SC_SAT> phi_t;
typedef sc_ufixed<XY_WL, XY_IWL, SC_RND, SC_SAT> xy_t;


SC_MODULE(Cordic)
{
    sc_in<bool> iStart;
    sc_out<bool> oRdy;
    sc_in<phi_t> iPhi;
    sc_out<xy_t> oX;
    sc_out<xy_t> oY;

    SC_CTOR(Cordic);

private:
    void calc();
};

#endif /* _CORDIC_H */
