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


SCA_TDF_MODULE(source){
    sca_tdf::sca_out<double> out{"out"};


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
    sca_tdf::sca_in<double> in{"in"};

    void processing() {
        cout << "Time: " << sc_time_stamp() << endl;
        cout << "OUT Value: " <<  in.read();
    }

    SCA_CTOR(drain){

    }
};



SCA_TDF_MODULE(splitter){

    sca_tdf::sca_in<double> in{"splitter_in"};
    sca_tdf::sca_out<double> out1{"splitter_out_1"};
    sca_tdf::sca_out<double> out2{"splitter_out_2"};
    SCA_CTOR(splitter) {

    }

  void processing() {
    double tmp = in.read();
    out1.write(tmp);
    out2.write(tmp);
  }
};

SC_MODULE(dc_motor_composition){

    sca_tdf::sca_in<double> in{"in"};
    sca_tdf::sca_out<double> out{"out"};

    sca_tdf::sca_signal<double> pid2pwm{"pid2pwn_signal"};
    sca_tdf::sca_signal<double> pwm2dcmotor{"pwm2dcmotor_signal"};
    sca_tdf::sca_signal<double> dcmotor2out{"dcmotor2out_signal"};
    sca_tdf::sca_signal<double> out2pid{"out2pid_signal"};

    pid* pid1;
    pwm* pwm1;
    dcmotor* dcmotor1;
    splitter* splitter1;

    // TODO DELAY


    SC_CTOR(dc_motor_composition) {
        pid1 = new pid("pid");
        pwm1 = new pwm("pid");
        dcmotor1 = new dcmotor("dcmotor");
        splitter1 = new splitter("splitter");

        pid1->in_ref(in);
        pid1->in_meas(out2pid);
        pid1->out(pid2pwm);

        pwm1->in(pid2pwm);
        pwm1->out(pwm2dcmotor);

        dcmotor1->in(pwm2dcmotor);
        dcmotor1->out(dcmotor2out);

        splitter1->in(dcmotor2out);
        splitter1->out1(out2pid);

        splitter1->out2(out);
    }

};

#endif //DCMOTOR_DC_MOTOR_COMPOSITION_H
