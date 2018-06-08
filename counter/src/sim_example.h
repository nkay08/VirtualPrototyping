#include <systemc.h>


SC_MODULE(counter)
{
   sc_out<int> out;
    
   sc_clock clk;
   int cnt;
   double th;
   double int_t;
   
    
    
    SC_HAS_PROCESS(counter);
    counter(sc_module_name n)
    :clk("clk", sc_time(2, SC_SEC))
    {
        SC_METHOD(processing);
        sensitive << clk;
        dont_initialize();
        cnt=0.0;
        int_t=0;
        th=2.0;
        
       
    }
    
    
    void processing()
    {
        
        double time=sc_time_stamp().to_seconds();
        
        int_t=0.5*time;
        
        if (int_t>=th)
        {
            cnt=cnt+1;
        }
        
        out.write(cnt);
    }
};

SC_MODULE(drain)
{
    sc_in<int> in;
    
    SC_HAS_PROCESS(drain);
    drain(sc_module_name n)
    {
        SC_METHOD(processing);
        sensitive << in;
        dont_initialize();
        
    }
    
    void processing()
    {
        cout << "Time: " << sc_time_stamp() << endl;
        cout << "Counter value: " <<  in.read();
        cout << endl;
        
    }
    
};



