#include <systemc.h>


SC_MODULE(dcmotor){

	sc_clock clk;


	void processing(){

	}

	SC_CTOR(dcmotor)//:clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;

	}

	/*
	SC_HAS_PROCESS(sbitCounter);
	sbitCounter(sc_module_name n):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		dont_initialize();
		


	}
	/**/


};

SC_MODULE(drain)
{
	sc_in<int> in;

	SC_CTOR(drain)
	{
		SC_METHOD(processing);
		sensitive << clk;
	}

	/*
	SC_HAS_PROCESS(drain);
	drain(sc_module_name n)
	{
		SC_METHOD(processing);
		sensitive << in;
		dont_initialize();

	}
	*/

	void processing()
	{
		cout << "Time: " << sc_time_stamp() << endl;
		cout << "Counter value: " <<  in.read();
		cout << endl;

	}

};
