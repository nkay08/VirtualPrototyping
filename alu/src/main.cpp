

#include "alu.h"
//#include "alu.cpp"


int sc_main(int argc, char * argv[]) {

sc_signal<bool> reset;
sc_signal<sc_bv<4>> op_sel;
sc_signal<sc_bv<32>> data1;
sc_signal<sc_bv<32>> data2;
sc_signal<sc_bv<32>> acc;
sc_signal<sc_bv<8>> stat;


alu alu("ALU");
alu.reset(reset);
alu.op_sel(op_sel);
alu.data1(data1);
alu.data2(data2);
alu.acc(acc);
alu.stat(stat);

resetMod resetMod("Reset");
resetMod.reset(reset);

opselMod opselMod("Op_Sel");
opselMod.op_sel(op_sel);

drain dr("Drain");
dr.acc(acc);
dr.data1(data1);
dr.data2(data2);
dr.stat(stat);

//init init("init");
//init.data1(data1);
//init.data2(data2);

sc_trace_file *tf=sc_create_vcd_trace_file("alu");

sc_trace(tf, op_sel, "opsel_output");
sc_trace(tf, reset, "reset_output");
sc_trace(tf, data1, "data1_output");
sc_trace(tf, data2, "data2_output");
sc_trace(tf, acc, "acc_output");
sc_trace(tf, stat, "stat_output");

//sc_start();
sc_start(10, SC_SEC);

return 0;

}
