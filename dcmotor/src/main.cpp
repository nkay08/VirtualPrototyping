

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
    bool composition_test = false;

    if (argc > 1){
        if (argc <= 4) {
            pid_test = std::stoi(argv[1]);
            pwm_test = std::stoi(argv[2]);
            composition_test = std::stoi(argv[3]);
        }
    }

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


    if (composition_test) {
        dc_motor_composition dcmc("dcmc");
        dcmc_source dcmc_source1("dcmc_source");
        dcmc_drain dcmc_drain1("dcmc_drain");

        sca_tdf::sca_signal<double> source2dcmc;
        sca_tdf::sca_signal<double> dcmc2drain;

        dcmc_source1.out(source2dcmc);

        dcmc.in(source2dcmc);
        dcmc.out(dcmc2drain);

        dcmc_drain1.in(dcmc2drain);

        sca_trace_file *tf=sca_create_vcd_trace_file("dcmc");
        sca_trace(tf, source2dcmc, "in_ref");
        sca_trace(tf, dcmc2drain, "out");

        sca_trace(tf, dcmc.pid1->diff2p, "diff");
        sca_trace(tf, dcmc.pid2pwm, "pid2pwm");
        sca_trace(tf, dcmc.pwm2dcmotor, "pwm2dcmotor");
        sca_trace(tf, dcmc.dcmotor2out, "dcmotor2out");
        sca_trace(tf, dcmc.out2pid, "in_meas");

        sc_start(300, SC_MS);
    }

return 0;
}
