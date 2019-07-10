/*
 * integral.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_INTEGRAL_H_
#define DCMOTOR_SRC_INTEGRAL_H_


#include <systemc.h>
#include "systemc-ams"







SCA_TDF_MODULE(integral){

	sca_tdf::sca_in<double> in{"in"};
	sca_tdf::sca_out<double> out{"out"};
	sca_util::sca_vector<double>	num, den;
	sca_tdf::sca_ltf_nd ltf;
	sca_util::sca_vector<double> s;
	double Ki;

	sca_tdf::sca_ltf_nd pid_integral;

	sca_core::sca_time t_step;

	void initialize(){
		num(0) = Ki;
		den(0) = 1.0 ;
	}

	void processing(){
		out.write( pid_integral( num, den, s, in.read() ) );
	}

	integral(
            sc_core::sc_module_name nm,
            double Ki_ = 4*M_PI
	        )
	:Ki(Ki_), t_step(sca_core::sca_time(0.01, sc_core::SC_MS))
	{
	}

    void set_attributes(){
//            set_timestep( t_step );
        accept_attribute_changes();
    }
};


#endif /* DCMOTOR_SRC_INTEGRAL_H_ */
