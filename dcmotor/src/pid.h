/*
 * pid.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PID_H_
#define DCMOTOR_SRC_PID_H_

#include <systemc.h>
#include "systemc-ams"
#include "proportional.h"
#include "integral.h"
#include "derivative.h"


SCA_TDF_MODULE(diff){
	sca_tdf::sca_in<double> in_ref{"in_ref"};
	sca_tdf::sca_in<double> in_meas{"in_meas"};

//    sca_tdf::sca_out<double> out;
    sca_tdf::sca_out<double> out_p{"out_p"};
    sca_tdf::sca_out<double> out_i{"out_i"};
    sca_tdf::sca_out<double> out_d{"out_d"};


	SCA_CTOR(diff){

	}

	void initialize(){

	}
	void processing(){
		double diff = in_ref.read() - in_meas.read();
		//		out.write( diff );
		out_p.write( diff );
        out_i.write( diff );
        out_d.write( diff );
	}

};



SCA_TDF_MODULE(pidsum){
	sca_tdf::sca_in<double> in_p{"sum_in_p"};
	sca_tdf::sca_in<double> in_i{"sum_in_i"};
	sca_tdf::sca_in<double> in_d{"sum_in_d"};
	sca_tdf::sca_out<double> out{"sum_out"};


	SCA_CTOR(pidsum){

	}

	void initialize(){

	}
	void processing(){
		out.write( in_p.read() + in_i.read() + in_d.read() );

	}

};

SC_MODULE(pid){
	sca_tdf::sca_in<double> in_ref{"pid_in_ref"};
	sca_tdf::sca_in<double> in_meas{"pid_in_meas"};
	sca_tdf::sca_out<double> out{"pid_out"};
	//	sca_tdf::sca_signal<double> diff2pid;
	sca_tdf::sca_signal<double> diff2p{"diff2p_signal"};
	sca_tdf::sca_signal<double> diff2i{"diff2i_signal"};
	sca_tdf::sca_signal<double> diff2d{"diff2d_signal"};
	sca_tdf::sca_signal<double> p2sum{"p2sum_signal"};
	sca_tdf::sca_signal<double> i2sum{"i2sum_signal"};
	sca_tdf::sca_signal<double> d2sum{"d2sum_signal"};

	proportional* p;
	integral* i;
	derivative* d;
	diff* diff1;
	pidsum* pidsum1;

	void initialize(){

	}

	void processing(){
	}

	SC_CTOR(pid)
	{
	    p = new proportional("p");
	    i = new integral("i");
	    d = new derivative("d");
	    diff1 = new diff("diff");
	    pidsum1 = new pidsum("pidsum");


		diff1->in_ref(in_ref);
		diff1->in_meas(in_meas);
		diff1->out_p(diff2p);
        diff1->out_i(diff2i);
        diff1->out_d(diff2d);

		//p->in(diff2pid);
		p->in(diff2p);
		p->out(p2sum);
		//		i->in(diff2pid);
		i->in(diff2i);
		i->out(i2sum);
		//		d->in(diff2pid);
		d->in(diff2d);
		d->out(d2sum);

		pidsum1->in_p(p2sum);
		pidsum1->in_i(i2sum);
		pidsum1->in_d(d2sum);
		pidsum1->out(out);


	}

	void setKp(double sKp){
		p->setKp(sKp);
	}

	void setKi(double sKi){
		i->setKi(sKi);
	}

	void setKd(double sKd){
		d->setKd(sKd);
	}
};

#endif /* DCMOTOR_SRC_PID_H_ */
