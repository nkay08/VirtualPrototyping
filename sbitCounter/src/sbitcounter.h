#include <systemc.h>
#include <sysc/datatypes/bit/sc_bv.h>
using namespace sc_dt;

SC_MODULE(sbitCounter){
	//inputs
	sc_clock clk;
	sc_in<bool> reset;
	sc_in<bool> count_en;
	sc_in<bool> ud_ctrl;

	//outputs
	sc_out<sc_bv<17>> cnt_out;
	sc_out<bool> ovf_intr;
	sc_out<bool> unf_intr;

	//internal
	sc_bv<17> cnt = "0";

	void processing();
	void add();
	void subtract();



	///*
	SC_CTOR(sbitCounter):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
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

SC_MODULE(resetMod){
	sc_clock clk;
	sc_out<bool> reset;
	bool shallReset;
	SC_CTOR(resetMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		shallReset = false;
	}

	void processing(){
		reset.write(shallReset);
	}
};

SC_MODULE(countEnMod){
	sc_clock clk;
	sc_out<bool> count_en;
	bool shallCount;
	SC_CTOR(countEnMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		shallCount = true;
	}

	void processing(){
		count_en.write(shallCount);
	}
};

SC_MODULE(udMod){
	sc_clock clk;
	sc_out<bool> ud_ctrl;
	bool ctrl;

	SC_CTOR(udMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		ctrl = false;
	}

	void processing(){
		ud_ctrl.write(ctrl);
	}
};


SC_MODULE(drain)
{
    sc_in<sc_bv<17>> in;
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
