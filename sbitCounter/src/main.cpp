/*
 * main.cpp
 *
 *  Created on: May 19, 2018
 *      Author: nkay
 */
//#include <systemc.h>
//#include "sbitcounter.h"
#include "sbitcounter.cpp"


//#include <sysc/datatypes/bit/sc_bv.h>


int sc_main(int argc, char * argv[]) {

    sc_signal<sc_bv<17>> sig;
    sc_signal<bool> ofl;
    sc_signal<bool> ufl;
    sc_signal<bool> res;
    sc_signal<bool> en;
    sc_signal<bool> ud;

    cout << "START" << endl;
   /*
    sc_bv<17> test = "111";
    cout << test << endl;
    cout << test[0] << endl;
    cout << test[1] << endl;
    cout << test[2] << endl;
    cout << test[3] << endl;
    /**/


    sbitCounter count("count");
    count.cnt_out(sig);
    count.ovf_intr(ofl);
    count.unf_intr(ufl);
    count.reset(res);
    count.count_en(en);
    count.ud_ctrl(ud);


    drain dr("dr");
    dr.in(sig);
    dr.ovf_intr(ofl);
    dr.unf_intr(ufl);

    resetMod resMod("ResetCtrl");
    resMod.reset(res);

    countEnMod countEnMod("EnableCnt");
    countEnMod.count_en(en);

    udMod udMod("UdCtrl");
    udMod.ud_ctrl(ud);


    cout << "Initialized" << endl;
    sc_start();
    //sc_start(10, SC_SEC);

    return 0;

}




