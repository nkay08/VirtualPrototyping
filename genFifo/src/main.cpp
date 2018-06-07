

#include "genFifo.h"


int sc_main(int argc, char * argv[]) {

intproducer p("producer");
p.setRandom(false);
intconsumer c("consumer");
c.setRandom(true);
genfifo<int> gf(50);
genfifo<int>* gfptr = &gf;
p.genFifo = gfptr;
c.genFifo = gfptr;


//sc_start(3, SC_SEC);
sc_start();
return 0;

}
