

//#include "alu.h"
#include "alu.cpp"


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

//sc_start();
sc_start(10, SC_SEC);

return 0;

}
