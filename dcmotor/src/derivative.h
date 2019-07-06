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

	void initialize(){

	}

	void setKd(double sKd){
		Kd = sKd;
	}

	void processing(){
		double temp = pid_derivative( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(derivative):Kd(1){

	}
};



#endif /* DCMOTOR_SRC_DERIVATIVE_H_ */
