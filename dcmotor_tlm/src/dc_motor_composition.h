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
#include "dcmotor_tlm.h"
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>


SCA_TDF_MODULE(dcmc_source){
    sca_tdf::sca_out<double> out{"out"};

    sca_core::sca_time t_step;

    double value;
    void processing() {
        double t = get_time().to_seconds() * 1000;

//        if  (t > 5) {
//            value = 0.1;
//        }
//        if ( t >10) {
//            value = 0.5;
//        }
//        if ( t > 15 ) {
//            value = 1.0;
//        }
//        if ( t > 25 ) {
//            value = 0.5;
//        }

        out.write(value);
//        cout << "Time: " << sc_time_stamp() << endl;
//        cout << "IN Value: " <<  tmp;
    }

    SCA_CTOR(dcmc_source): value(1.0), t_step(sca_core::sca_time(0.01, sc_core::SC_MS)){

    }

    void set_attributes(){
//        set_timestep( t_step );
        accept_attribute_changes();
    }
};


SCA_TDF_MODULE(dcmc_drain){
    sca_tdf::sca_in<double> in{"in"};

    sca_core::sca_time t_step;

    void processing() {
//        cout << "Time: " << sc_time_stamp() << endl;
//        cout << "OUT Value: " <<  in.read();
    }

    SCA_CTOR(dcmc_drain): t_step(sca_core::sca_time(0.01, sc_core::SC_MS)){

    }

    void set_attributes(){
//        set_timestep( t_step );
        accept_attribute_changes();
    }
};



SCA_TDF_MODULE(splitter){

    sca_tdf::sca_in<double> in{"splitter_in"};
    sca_tdf::sca_out<double> out1{"splitter_out_1"};
    sca_tdf::sca_out<double> out2{"splitter_out_2"};

    sca_core::sca_time t_step;

    SCA_CTOR(splitter):t_step(sca_core::sca_time(0.01, sc_core::SC_MS)) {

    }

  void processing() {
    double tmp = in.read();
    out1.write(tmp);
    out2.write(tmp);
  }

    void set_attributes(){
        accept_attribute_changes();
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

    dcmotor_initiator* initiator;
    dcmotor_target* target;

    // TODO DELAY

    dc_motor_composition(
            sc_core::sc_module_name nm,
            bool dynamic_pwm,
            double h0_ = 15.0,
            double w0_ = 20 * M_PI,
            double Kp_ = 1.0/15.0,
            double Ki_ = 4*M_PI,
            double Kd_ = 0.0,
            double v_drv_ = 1.0,
            const sca_core::sca_time& t_period_ = sca_core::sca_time(5.0, sc_core::SC_MS),
            const sca_core::sca_time& t_ramp_ = sca_core::sca_time(0.05, sc_core::SC_MS),
            const sca_core::sca_time& t_step_ = sca_core::sca_time(0.01, sc_core::SC_MS),
            bool trace = false,
            bool test_tlm = false
            )
        {
            pid1 = new pid("pid", Kp_, Ki_, Kd_);
            pwm1 = new pwm("pwm", v_drv_,  t_period_, t_ramp_, t_step_, dynamic_pwm);
            dcmotor1 = new dcmotor("dcmotor", h0_, w0_);
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

            initiator = new dcmotor_initiator("initiator");
            target = new dcmotor_target("target");
            target->pid1 = pid1;

            if (test_tlm)
            {
                initiator->run_test = true;
            }

            initiator->socket.bind(target->socket);

            if (trace) {
                sca_trace_file *tf=sca_create_vcd_trace_file("dcmc_inner");
                sca_trace(tf, out2pid, "out2pid");
                sca_trace(tf, pid2pwm, "pid2pwm");
                sca_trace(tf, pwm2dcmotor, "pwm2dcmotor");
                sca_trace(tf, dcmotor2out, "dcmotor2out");
            }


        }

};

#endif //DCMOTOR_DC_MOTOR_COMPOSITION_H
