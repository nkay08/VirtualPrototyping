//
// Created by nkay on 7/4/19.
//

#ifndef DCMOTOR_DC_MOTOR_COMPOSITION_H
#define DCMOTOR_DC_MOTOR_COMPOSITION_H

#include <systemc.h>
#include <systemc-ams.h>
#include "pid.h"
#include "pwm.h"
#include "dcmotor.h"

SC_MODULE(dc_motor_composition){

    sca_tdf::sca_in<double> in;
    sca_tdf::sca_out<double> out;

    sca_tdf::sca_signal<double> in2pid;
    sca_tdf::sca_signal<double> pid2pwm;
    sca_tdf::sca_signal<double> pwm2dcmotor;
    sca_tdf::sca_signal<double> dcmotor2pid;


    pid* pid1;
    pwm* pwm1;
    dcmotor* dcmotor1;

    // TODO DELAY


    SC_CTOR(dc_motor_composition) {
        pid1 = new pid("pid");
        pwm1 = new pwm("pid");
        dcmotor1 = new dcmotor("dcmotor");



        pid1->in_ref(in2pid);
        pid1->out(pid2pwm);

        pwm1->in(pid2pwm);
        pwm1->out(pwm2dcmotor);

        dcmotor1->in(pwm2dcmotor);
        dcmotor1->out(dcmotor2pid);

        out(dcmotor2pid);
    }

};


SCA_TDF_MODULE(source){
    sca_tdf::sca_out<double> out;


    void processing() {
        double tmp = 5.0;
        out.write(tmp);
        cout << "Time: " << sc_time_stamp() << endl;
        cout << "IN Value: " <<  tmp;
    }

    SCA_CTOR(source){

    }
};


SCA_TDF_MODULE(drain){
    sca_tdf::sca_in<double> in;

    void processing() {
        cout << "Time: " << sc_time_stamp() << endl;
        cout << "OUT Value: " <<  in.read();
    }

    SCA_CTOR(drain){

    }
};


#endif //DCMOTOR_DC_MOTOR_COMPOSITION_H
