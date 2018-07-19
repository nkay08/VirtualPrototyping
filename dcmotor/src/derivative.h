/*
 * derivative.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_DERIVATIVE_H_
#define DCMOTOR_SRC_DERIVATIVE_H_

#include <systemc.h>

SCA_TDF_MODULE(derivative){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den, s;
	double Kp;

	sca_tdf::sca_ltf_nd pid_derivative;

	void initialize(){

	}

	void setKd(float sKd){
		Kd = sKd;
	}

	void processing(){
		float temp = pid_derivative( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(derivative):Kp(1/15.0){

	}
};



#endif /* DCMOTOR_SRC_DERIVATIVE_H_ */
