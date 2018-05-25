#include <systemc.h>


SC_MODULE(alu){

	sc_clock clk;
	sc_in<sc_bv<3>> op_sel;
	sc_in<bool> reset;

	sc_inout<sc_bv<32>> data1;
	sc_inout<sc_bv<32>> data2;
	sc_out<sc_bv<32>> acc;
	sc_out<sc_bv<8>> stat;

	void processing();

	void aluand();
	void aluor();
	void aluxor();
	void aluadd();
	void alumul();
	void aludiv();
	void aluror();
	void alurol();

	SC_CTOR(alu):clk("clk", sc_time(1, SC_SEC))
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


SC_MODULE(resetMod){
	sc_out<bool> reset;
	sc_clock clk;
	bool shallReset;


	SC_CTOR(resetMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		shallReset = false;
		sensitive << clk;
	}

	void processing(){

		reset.write(shallReset);

	}

};

SC_MODULE(opselMod){

	sc_out<sc_bv<3>> op_sel;
	sc_clock clk;
	sc_bv<3> sel;

	SC_CTOR(opselMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;

		sel = "001"
	}

	void processing(){

		op_sel.write(sel);

	}

}


SC_MODULE(drain)
{
	sc_in<sc_bv<8>> stat;
	sc_in<sc_bv<32>> acc;
	sc_in<sc_bv<32>> data1
	sc_in<sc_bv<32>> data2;

	SC_HAS_PROCESS(drain);
	drain(sc_module_name n)
	{
		SC_METHOD(processing);
		sensitive << acc << stat << data1 << data2;
		dont_initialize();

	}

	void processing()
	{
		cout << "Time: " << sc_time_stamp() << endl;
		cout << "Counter value: " <<  acc.read();
		cout << endl;

	}

};
