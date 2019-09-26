/*
 * derivative.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_DERIVATIVE_H_
#define DCMOTOR_SRC_DERIVATIVE_H_

#include <systemc.h>
#include "systemc-ams"

SCA_TDF_MODULE(derivative){
	sca_tdf::sca_in<double> in{"in"};
	sca_tdf::sca_out<double> out{"out"};
	sca_util::sca_vector<double>	num, den, s;
	double Kd;

	sca_tdf::sca_ltf_nd pid_derivative;
	sca_core::sca_time t_step;

	void initialize(){
        num(0) = Kd;
        den(0) = 1.0 ;
	}

	void processing(){
		double temp = pid_derivative( num, den, s, in.read() );
		out.write(temp);
	}

	derivative(
            sc_core::sc_module_name nm,
            double Kd_ = 1.0
	        )
	:Kd(Kd_), t_step(sca_core::sca_time(0.01, sc_core::SC_MS))
	{
	}

    void set_attributes(){
//            set_timestep( t_step );
        accept_attribute_changes();
    }

};



#endif /* DCMOTOR_SRC_DERIVATIVE_H_ */
