

//#include "dcmotor.h"
//#include "test.h"
//#include "pwm.h"
#include "pid.h"
#include "proportional.h"
#include "dc_motor_composition.h"
#include "pwm.h"
#include <systemc-ams.h>
#include <systemc.h>

int sc_main(int argc, char * argv[]) {

    bool pid_test = false;
    bool pwm_test = false;

    if (pwm_test) {
        pwm pwm1("pwm");
        pwm_source pwm_source1("pwm_source");
        pwm_drain pwm_drain1("pwm_drain");

        sca_tdf::sca_signal<double> source2pwm;
        sca_tdf::sca_signal<double> pwm2drain;

        pwm_source1.out(source2pwm);
        pwm1.in(source2pwm);
        pwm1.out(pwm2drain);
        pwm_drain1.in(pwm2drain);

        sca_trace_file *tf=sca_create_vcd_trace_file("pwm");
        sca_trace(tf, source2pwm, "source2pwm");
        sca_trace(tf, pwm2drain, "pwm2drain");

        sc_start(30, SC_MS);
    }

    if (pid_test) {
        pid pid1("pid");
        pid_source pid_source1("pid_source");
        pid_drain pid_drain1("pid_drain");

        sca_tdf::sca_signal<double> source_ref2pid;
        sca_tdf::sca_signal<double> source_meas2pid;
        sca_tdf::sca_signal<double> pid2drain;

        pid_source1.out_ref(source_ref2pid);
        pid_source1.out_meas(source_meas2pid);

        pid1.in_ref(source_ref2pid);
        pid1.in_meas(source_meas2pid);
        pid1.out(pid2drain);

        pid_drain1.in(pid2drain);

        sca_trace_file *tf=sca_create_vcd_trace_file("pid");
        sca_trace(tf, source_ref2pid, "source_ref2pid");
        sca_trace(tf, source_meas2pid, "source_meas2pid");
        sca_trace(tf, pid2drain, "pid2drain");

        sc_start(50, SC_MS);
    }



return 0;
}
