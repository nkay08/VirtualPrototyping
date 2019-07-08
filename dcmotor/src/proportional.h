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
	sca_tdf::sca_in<double> in{"in"};
	sca_tdf::sca_out<double> out{"out"};
	sca_util::sca_vector<double>	num, den, s;
	sca_tdf::sca_ltf_nd ltf;

	double Kp;


	proportional(
            sc_core::sc_module_name nm,
	        double Kp_ = 1.0/15.0
	        )
	:Kp(Kp_)
	{
	}

    void initialize(){
        num(0)=Kp;
        den(0)=1;
    }

    void processing(){
        double temp = ltf( num,den, s, in.read() );
        out.write(temp);
    }

};


SCA_TDF_MODULE(p_source){
        sca_tdf::sca_out<double> out;

        double value;

        void processing(){
            double t = get_time().to_seconds() * 1000;

            if  (t > 10) {
                value = 0.1;
            }
            if ( t > 20) {
                value = 1;
            }

            out.write(value);
//            cout << "Time: " << sc_time_stamp() << endl;
//            cout << "IN Value: " <<  value;
        }

        void set_attributes(){
            accept_attribute_changes();
        }

        SCA_CTOR(p_source):value(0.7)
        {
        }
};


SCA_TDF_MODULE(p_drain) {
        sca_tdf::sca_in<double> in;

        void set_attributes(){
            accept_attribute_changes();
        }

        void processing() {
            double tmp = in.read();
//        cout << "Time: " << sc_time_stamp() << endl;
//        cout << "OUT Value: " <<  tmp;
        };

        SCA_CTOR(p_drain){
        }
};


#endif /* DCMOTOR_SRC_PROPORTIONAL_H_ */
