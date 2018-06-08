

#include "genFifo.h"


int sc_main(int argc, char * argv[]) {
sc_signal<int> wr;
sc_signal<bool> wrb;
sc_signal<bool> wrs;
sc_signal<bool> wra;
sc_signal<int> re;
sc_signal<bool> reb;
sc_signal<bool> res;
sc_signal<bool> rea;

intproducer p("producer");
p.setRandom(true);
intconsumer c("consumer");
c.setRandom(true);
genfifo<int> gf(50);
genfifo<int>* gfptr = &gf;
p.genFifo = gfptr;
c.genFifo = gfptr;

p.data(wr);
p.block(wrb);
p.succio(wrs);
p.startaction(wra);

c.data(re);
c.block(reb);
c.succio(res);
c.startaction(rea);


sc_trace_file *tf=sc_create_vcd_trace_file("genfifo");

sc_trace(tf, wr, "write_output");
sc_trace(tf, wrb, "write_blocking");
sc_trace(tf, wrs, "write_success");
sc_trace(tf, wra, "write_started");
sc_trace(tf, re, "read_output");
sc_trace(tf, reb, "read_blocking");
sc_trace(tf, res, "read_success");
sc_trace(tf, rea, "read_started");

sc_start(100, SC_NS);
//sc_start();
return 0;

}
