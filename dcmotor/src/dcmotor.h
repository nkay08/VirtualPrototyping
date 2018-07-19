#ifndef DCMOTOR_SRC_DCMOTOR_H_
#define DCMOTOR_SRC_DCMOTOR_H_

#include <systemc.h>


SCA_TDF_MODULE(dcmotor){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_tdf::sca_ltf_nd ltf;


	// sc_clock clk;


	void initialize(){

	}


	void processing(){

	}

	SCA_CTOR(dcmotor)//:clk("clk", sc_time(1, SC_SEC))
	{

	}




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
