#ifndef DCMOTOR_SRC_DCMOTOR_H_
#define DCMOTOR_SRC_DCMOTOR_H_

#include <systemc.h>
#include <systemc-ams.h>

SCA_TDF_MODULE(dcmotor) {
    sca_tdf::sca_in <double > in{"in"};
    sca_tdf::sca_out<double> out{"out"};


    //filter equation objects
    sca_tdf::sca_ltf_nd ltf;
    sca_util::sca_vector<double> s;

    double h0;
    double w0;
    sca_util::sca_vector<double>  num;
    sca_util::sca_vector<double>  den;

    void initialize()
    {
        num(0) = h0;
        den(0) = 1.0;
        den(1) = 1/w0;
    }

    void processing() {
        double tmp;
        tmp = ltf(num, den, s, in.read());
        out.write(tmp);
    }
    SCA_CTOR(dcmotor) { //default parameter
        h0 = 15.0;
        w0= 20 * M_PI;
    }


};



#endif /* DCMOTOR_SRC_INTEGRAL_H_ */