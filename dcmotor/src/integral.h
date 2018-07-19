/*
 * integral.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_INTEGRAL_H_
#define DCMOTOR_SRC_INTEGRAL_H_


#include <systemc.h>







SCA_TDF_MODULE(integral){

	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den;
	sca_tdf::sca_ltf_nd ltf;
	sca_util::sca_vector<float> s;
	double Ki;

	sca_tdf::sca_ltf_nd pid_integral;

	void initialize(){
		num(0) = Ki;
		den(0) = 1.0 ;
	}

	void setKi(float sKi){
		Ki = sKi;
	}

	void processing(){
		float infloat = in.read();
		out.write( ltf( num,den,in.read() ) );
	}

	SCA_CTOR(integral):Ki(4*M_PI){

	}
};


#endif /* DCMOTOR_SRC_INTEGRAL_H_ */
