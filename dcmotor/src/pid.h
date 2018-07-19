/*
 * pid.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PID_H_
#define DCMOTOR_SRC_PID_H_

#include <systemc.h>
#include "proportional.h"
#include "integral.h"
#include "derivative.h"


SCA_TDF_MODULE(diff){
	sca_tdf::sca_in<float> in_ref;
	sca_tdf::sca_in<float> in_meas;
	//	sca_tdf::sca_out<float> out;
	sca_tdf::sca_out<float> out_p;
	sca_tdf::sca_out<float> out_i;
	sca_tdf::sca_out<float> out_d;


	SCA_CTOR(diff){

	}

	void initialize(){

	}
	void processing(){
		float diff = in_ref.read() - in_meas.read();
		//		out.write( diff );
		out_p.write( diff );
		out_i.write( diff );
		out_d.write( diff );
	}

};


SCA_TDF_MODULE(pidsum){
	sca_tdf::sca_in<float> in_p;
	sca_tdf::sca_in<float> in_i;
	sca_tdf::sca_in<float> in_d;
	sca_tdf::sca_out<float> out;


	SCA_CTOR(pidsum){

	}

	void initialize(){

	}
	void processing(){
		out.write( in_p.read() + in_i.read() + in_d.read() );

	}

};

SCA_TDF_MODULE(pid){
	sca_tdf::sca_in<float> in_ref;
	sca_tdf::sca_in<float> in_meas;
	sca_tdf::sca_out<float> out;
	//	sca_tdf::sca_signal<float> diff2pid;
	sca_tdf::sca_signal<float> diff2p;
	sca_tdf::sca_signal<float> diff2i;
	sca_tdf::sca_signal<float> diff2d;
	sca_tdf::sca_signal<float> p2sum;
	sca_tdf::sca_signal<float> i2sum;
	sca_tdf::sca_signal<float> d2sum;

	proportional* p;
	integral* i;
	derivative* d;
	diff* diff1;
	pidsum* pidsum1;

	void initialize(){

	}

	void processing(){



	}

	SCA_CTOR(pid):p("p"),i("i"),d("d"),diff1("diff"),pidsum1("pidsum"){
		diff1->in_ref(in_ref);
		diff1->in_meas(in_meas);
		diff1->out(diff2pid);
		//p->in(diff2pid);
		p->in(diff2p);
		p->out(p2sum);
		//		i->in(diff2pid);
		i->in(diff2i);
		i->out(i2sum);
		//		d->in(diff2pid);
		d->in(diff2d);
		d->out(d2sum);
		pidsum1->p_in(p2sum);
		pidsum1->i_in(i2sum);
		pidsum1->d_in(d2sum);
		pidsum1->out(out);

	}

	void setKp(float sKp){
		p->setKp(sKp);
	}

	void setKi(float sKi){
		i->setKi(sKi);
	}

	void setKd(float sKd){
		d->setKd(sKd);
	}
};

#endif /* DCMOTOR_SRC_PID_H_ */
