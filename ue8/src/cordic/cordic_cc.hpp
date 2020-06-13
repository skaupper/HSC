/*****************************************************************************/
/**
 * @file    cordic.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Header of cycle accurate Cordic algorithm
 */
/*****************************************************************************/

#ifndef _CORDIC_CC_HPP_
#define _CORDIC_CC_HPP_

#include "cordic.hpp"



SC_MODULE(cordic_cc)
{
  sc_in<bool> iClk;
  sc_in<bool> inRst;
  sc_in<bool> iStart;
  sc_out<bool> oRdy;
  sc_in<phi_t> iPhi;
  sc_out<xy_t> oX;
  sc_out<xy_t> oY;

  SC_CTOR(cordic_cc);

private:
    void calc();
};

typedef cordic_cc CordicCc;


#endif /* _CORDIC_CC_HPP_ */
