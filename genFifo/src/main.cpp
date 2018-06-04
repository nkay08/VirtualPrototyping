

#include "genFifo.h"
#include "genFifo.cpp"

int sc_main(int argc, char * argv[]) {

producer p("producer");
consumer c("consumer");
genfifo<int> gf("genfifo");

p.genFifo = &gf;
c.genFifo = &gf;

sc_start();
return 0;

}
