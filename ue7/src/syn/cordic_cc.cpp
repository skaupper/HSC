/*****************************************************************************/
/**
 * @file    cordic.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   Implementation of a cycle accurate Cordic algorithm
 */
/*****************************************************************************/

#include "cordic_cc.hpp"
#include <cmath>

using namespace std;


static void calcAtan(double atanLut[])
{
#pragma HLS INLINE
    for (int i = 0; i < ITERATIONS; ++i) {
        atanLut[i] = atan(pow(2, -i));
    }
}

static void calcK(double *K)
{
    *K = 1;

    for (int i = 0; i < ITERATIONS; ++i) {
        *K = *K / sqrt(1 + pow(2, -2 * i));
    }
}


CordicCc::CordicCc(sc_module_name name) : sc_module(name) {
    SC_CTHREAD(calc, iClk.pos());
    reset_signal_is(inRst, false);
}

void CordicCc::calc() {
#pragma HLS RESOURCE variable=iStart core=AXI4LiteS metadata="-bus_bundle cordic_if"
#pragma HLS RESOURCE variable=iPhi core=AXI4LiteS metadata="-bus_bundle cordic_if"
#pragma HLS RESOURCE variable=oX core=AXI4LiteS metadata="-bus_bundle cordic_if"
#pragma HLS RESOURCE variable=oY core=AXI4LiteS metadata="-bus_bundle cordic_if"
#pragma HLS RESOURCE variable=oRdy core=AXI4LiteS metadata="-bus_bundle cordic_if"

    //
    // Local types used
    //

    typedef sc_fixed<28, 2> z_calc_t;
    typedef sc_fixed<21, 2> xy_calc_t;


    //
    // Init ROMs
    //

    static double ATAN_LUT[ITERATIONS];
    static double K;
#pragma HLS RESOURCE variable=ATAN_LUT core=ROM_nP
    calcAtan(ATAN_LUT);
    calcK(&K);


    //
    // Reset routines
    //

    oRdy.write(1);
    oY.write(0);
    oX.write(0);

    wait();


    //
    // Variables used
    //

    // Control state
    bool calcStarted  = false;
    short currentIter = 0;


    // Intermediate values
    phi_t phi;
    xy_calc_t x;
    xy_calc_t x_tmp;
    xy_calc_t y;
    xy_calc_t y_tmp;
    z_calc_t z;
    int d;


    while (true) {
    	wait();

        // If no calculation is started and iStart is low, do nothing.
        if (iStart == 0 && !calcStarted) {
            // cout << "Wait for iStart... @" << sc_time_stamp() << endl;
            continue;
        }

        // Start a calculation
        if (iStart == 1 && !calcStarted) {
            currentIter = 0;

            // Initial values
            phi   = iPhi.read();
            x     = K;
            x_tmp = K;
            y     = 0;
            z     = phi;
            d     = 1;

            // Deassert oRdy until the calculation finished
            oRdy.write(0);
            calcStarted = true;
        }

        // cout << "## Start Cordic Calculation Iteration " << currentIter << " @ " << sc_time_stamp() << endl;

        //
        // Do calculation
        //

        if (z >= 0) {
            d = 1;
        } else {
            d = -1;
        }

        z_calc_t tmpAtan = ATAN_LUT[currentIter];
        if (d == -1) {
            tmpAtan = -tmpAtan;
        }

        z     = z - tmpAtan;
        x_tmp = (y * d) >> currentIter;
        y_tmp = (x * d) >> currentIter;
        x     = x - x_tmp;
        y     = y + y_tmp;


        // Check if the calculation ended
        if (currentIter == ITERATIONS - 1) {
            oX.write(x);
            oY.write(y);
            oRdy.write(1);
            calcStarted = false;
        }

        currentIter++;
    }
}
