

#include "genFifo.h"


int sc_main(int argc, char * argv[]) {

intproducer p("producer");
intconsumer c("consumer");
genfifo<int> gf("genfifo");
genfifo<int>* gfptr = &gf;
p.genFifo = gfptr;
c.genFifo = gfptr;

sc_start();
return 0;

}
