

//#include "dcmotor.h"
//#include "test.h"
//#include "pwm.h"
#include "pid.h"
#include "dc_motor_composition.h"
#include <systemc-ams.h>
#include <systemc.h>

int sc_main(int argc, char * argv[]) {


//dc_motor_composition dcmc("dcmc");
pid pid1("pid");

drain drain1("drain");
source source1("source");

sca_tdf::sca_signal<double> source2dcmc;
sca_tdf::sca_signal<double> dcmc2drain;

source source2("source2");
sca_tdf::sca_signal<double> source22dcmc;
source2.out(source22dcmc);
pid1.in_meas(source22dcmc);

pid1.in_ref(source2dcmc);
pid1.out(dcmc2drain);


//dcmc.in(source2dcmc);
//dcmc.out(dcmc2drain);

source1.out(source2dcmc);
drain1.in(dcmc2drain);



sc_start(10, SC_SEC);

return 0;
}
