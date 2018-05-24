/*
 * main.cpp
 *
 *  Created on: May 19, 2018
 *      Author: nkay
 */
#include "sbitcounter.h"
#include "sbitcounter.cpp"


int sc_main(int argc, char * argv[]) {

    sc_signal<int> sig;
    sc_signal<bool> ofl;
    sc_signal<bool> ufl;
    cout << "START";
    sbitCounter count("count");
    count.cnt_out(sig);
    count.ovf_intr(ofl);
    count.unf_intr(ufl);


    drain dr("dr");
    dr.in(sig);
    dr.ovf_intr(ofl);
    dr.unf_intr(ufl);

    //sc_start(10, SC_SEC);
    sc_start();

    return 0;

}




