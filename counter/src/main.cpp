#include "sim_example.h"


int sc_main(int argc, char * argv[]) {
    
    sc_signal<int> sig;
    
    counter count("count");
    count.out(sig);
    
    drain dr("dr");
    dr.in(sig);
    
    // visualise signals in gtkwave
    
    
    sc_trace_file *tf=sc_create_vcd_trace_file("counter");
    
    sc_trace(tf, sig, "counter_output");
    
    sc_start(10, SC_SEC);
    

}
