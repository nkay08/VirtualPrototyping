/*
 * proportional.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PROPORTIONAL_H_
#define DCMOTOR_SRC_PROPORTIONAL_H_

#include <systemc.h>
#include "systemc-ams"

SCA_TDF_MODULE(proportional){
	sca_tdf::sca_in<double> in;
	sca_tdf::sca_out<double> out;
	sca_util::sca_vector<double>	num, den, s;
	sca_tdf::sca_ltf_nd ltf;

	double Kp;

	void initialize(){
		num(0)=Kp;
		den(0)=1;
	}

	void setKp(double sKp){
		Kp = sKp;
	}

	void processing(){
		//out.write( in.read() * Kp);
		double temp = ltf( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(proportional):Kp(1.0/15.0){

	}
};


#endif /* DCMOTOR_SRC_PROPORTIONAL_H_ */
