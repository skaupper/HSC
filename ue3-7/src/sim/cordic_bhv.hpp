/*****************************************************************************/
/**
 * @file    cordic_bhv.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Header of a behavioural model of the Cordic algorithm
 */
/*****************************************************************************/

#ifndef _CORDIC_H
#define _CORDIC_H

#include "cordic.hpp"


SC_MODULE(CordicBhv)
{
    sc_in<bool> iStart;
    sc_out<bool> oRdy;
    sc_in<phi_t> iPhi;
    sc_out<xy_t> oX;
    sc_out<xy_t> oY;

    SC_CTOR(CordicBhv);

private:
    void calc();
};

#endif /* _CORDIC_H */
