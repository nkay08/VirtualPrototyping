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

    double h0, w0;
    sca_util::sca_vector<double>  num;
    sca_util::sca_vector<double>  den;

    sca_core::sca_time t_step;

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

    dcmotor(
            sc_core::sc_module_name nm,
            double h0_ = 15.0,
            double w0_ = 20 *  M_PI
            )
            : h0(h0_), w0(w0_), t_step(sca_core::sca_time(0.01, sc_core::SC_MS))
            {
            }

    void set_attributes(){
//            set_timestep( t_step );
        out.set_delay(1);
        accept_attribute_changes();
    }


};



#endif /* DCMOTOR_SRC_INTEGRAL_H_ */