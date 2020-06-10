#ifndef TOP_H
#define TOP_H

#include "CordicTb.h"
#include "../syn/cordic_cc.hpp"


SC_MODULE(Top)
{
  sc_clock mClk;
  sc_signal<bool> mReset;

  sc_signal<bool> mStartCordic;
  sc_signal<bool> mCordicReady;
  sc_signal<phi_t> mPhi;
  sc_signal<xy_t> mX;
  sc_signal<xy_t> mY;

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

    mTb->oStart(mStartCordic);
    mTb->iRdy(mCordicReady);
    mTb->oPhi(mPhi);
    mTb->iX(mX);
    mTb->iY(mY);

    SC_THREAD(generateReset);
  }



private:
  void generateReset()
  {
      mReset = 1;
      wait(127, SC_NS);
      mReset = 0;
  }

  CordicCc *mCordic;
  CordicTb *mTb;
};

#endif
