#include "sim_example.h"


int sc_main(int argc, char * argv[]) {
    
    sc_signal<double> sig;
    
    counter count("count");
    count.out(sig);
    
    drain dr("dr");
    dr.in(sig);
    
    sc_start(10, SC_SEC);
    

}
