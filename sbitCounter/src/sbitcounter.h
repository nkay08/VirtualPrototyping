#include <systemc.h>
#include <sysc/datatypes/bit/sc_bv.h>
using namespace sc_dt;

SC_MODULE(sbitCounter){
	//inputs
	sc_clock clk;
	//sc_in<bool> reset;
	//sc_in<bool> count_en;
	//sc_in<bool> ud_ctrl;

	//outputs
	sc_out<int> cnt_out;
	sc_out<bool> ovf_intr;
	sc_out<bool> unf_intr;

	//internal
	unsigned int cnt;
	void processing();
	//const unsigned int max = 131072;
	const unsigned int max = 5;
	 ///*
	SC_CTOR(sbitCounter)//:clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		cnt=0;
	}
	/**/

	/*
	SC_HAS_PROCESS(sbitCounter);
	sbitCounter(sc_module_name n):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		dont_initialize();
		cnt=0;


	}
	/**/



};

SC_MODULE(drain)
{
    sc_in<int> in;
    sc_in<bool> ovf_intr;
	sc_in<bool> unf_intr;

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
        if(ovf_intr.read()) cout << " Overflow";
        if(unf_intr.read()) cout << " Underflow";
        cout << endl;

    }

};
