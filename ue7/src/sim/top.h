#ifndef TOP_H
#define TOP_H

#include "CordicTb.h"
#include "../syn/cordic_cc.hpp"


SC_MODULE(Top)
{
  // Signals for Cordic and TB
  sc_clock mClk;
  sc_signal<bool> mReset;

  sc_signal<bool> mStartCordic;
  sc_signal<bool> mCordicReady;
  sc_signal<phi_t> mPhi;
  sc_signal<xy_t> mX;
  sc_signal<xy_t> mY;


  // CTOR
  SC_HAS_PROCESS(Top);

  Top(sc_module_name name) : sc_module(name), mClk("clk", 10, SC_NS)
  {
    mCordic = new CordicCc("CORDIC");
    mTb = new CordicTb("Testbench");

    mCordic->iClk(mClk);
    mCordic->inRst(mReset);
    mCordic->iStart(mStartCordic);
    mCordic->oRdy(mCordicReady);
    mCordic->iPhi(mPhi);
    mCordic->oX(mX);
    mCordic->oY(mY);

    mTb->inRst(mReset);
    mTb->oStart(mStartCordic);
    mTb->iRdy(mCordicReady);
    mTb->oPhi(mPhi);
    mTb->iX(mX);
    mTb->iY(mY);

    SC_THREAD(generateReset);


    // Configure trace file
    sc_trace_file *tf;
    tf = sc_create_vcd_trace_file("bin/cordic_trace");
    tf->delta_cycles(true);
    sc_trace(tf, mClk, "iClk");
    sc_trace(tf, mReset, "inRst");
    sc_trace(tf, mStartCordic, "iStart");
    sc_trace(tf, mCordicReady, "oRdy");
    sc_trace(tf, mPhi, "iPhi");
    sc_trace(tf, mX, "oX");
    sc_trace(tf, mY, "oY");
  }



private:
  void generateReset()
  {
      mReset = 0;
      wait(127, SC_NS);
      mReset = 1;
  }

  CordicCc *mCordic;
  CordicTb *mTb;
};

#endif
