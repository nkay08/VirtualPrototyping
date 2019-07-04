//
// Created by nkay on 7/4/19.
//

#ifndef DCMOTOR_TEST_H
#define DCMOTOR_TEST_H

#include <systemc-ams.h>


SCA_TDF_MODULE(prefi_ac) {sca_tdf::sca_in < double > in; sca_tdf::sca_out<double> out;
    //control / DE signal from SystemC
    // (connected to sc_signal<bool>)
    sca_tdf::sc_in<bool> fc_high;
    double fc0, fc1, v_max;
    //filter equation objects
    sca_tdf::sca_ltf_nd ltf_0, ltf_1;
    sca_util::sca_vector<double> a0, a1, b;
    sca_util::sca_vector<double> s;

    void initialize()
    {
        const double r2pi = M_1_PI * 0.5;
        b(0) = 1.0;
        a1(0) = a0(0) = 1.0;
        a1(1) = r2pi / fc0;
        a1(1) = r2pi / fc1;
    }

    void processing() {
        double tmp;
        //high or low cut-off freq.
        if(fc_high) tmp = ltf_1(b,a1,s,in);
        else tmp = ltf_0(b,a0,s,in);
        //output value limitation
        if (tmp > v_max) tmp = v_max;
        else if (tmp < -v_max) tmp = -v_max;
        out.write(tmp);
    }
    SCA_CTOR(prefi_ac) { //default parameter values fc0 = 1.0e3; fc1=1.0e5; v_max=1.0; }
    }
};


#endif //DCMOTOR_TEST_H
