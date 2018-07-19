/*
 * proportional.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PROPORTIONAL_H_
#define DCMOTOR_SRC_PROPORTIONAL_H_

#include <systemc.h>

SCA_TDF_MODULE(proportional){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den, s;
	sca_tdf::sca_ltf_nd ltf;

	float Kp;

	void initialize(){
		num(0)=Kp;
		den(0)=1;
	}

	void setKp(float sKp){
		Kp = sKp;
	}

	void processing(){
		//out.write( in.read() * Kp);
		float temp = ltf( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(proportional):Kp(1){

	}
};


#endif /* DCMOTOR_SRC_PROPORTIONAL_H_ */
