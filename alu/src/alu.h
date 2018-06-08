#include <systemc.h>


SC_MODULE(alu){
	bool initonce;
	sc_clock clk;
	sc_in<sc_bv<4>> op_sel;
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
		initonce = true;
		dont_initialize();
		//data1.write("1001");
		//data2.write("101010");

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

SC_MODULE(init){
	sc_clock clk;
	sc_out<sc_bv<32>> data1;
	sc_out<sc_bv<32>> data2;
	bool once;

	SC_CTOR(init):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		once = true;
		dont_initialize();
	}

	void processing(){
		if(once){
			sc_bv<32> d1 = "10001001010";
			sc_bv<32> d2 = "1001111001";
			data1.write(d1);
			data2.write(d2);
			once = false;
		}

	}

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
		dont_initialize();
	}

	void processing(){
		int v2 = rand() % 100;
		if(v2 < 2) shallReset = true;
		else shallReset = false;
		reset.write(shallReset);

	}

};

SC_MODULE(opselMod){

	sc_out<sc_bv<4>> op_sel;
	sc_clock clk;
	sc_bv<4> sel;

	SC_CTOR(opselMod):clk("clk", sc_time(1, SC_SEC))
	{
		SC_METHOD(processing);
		sensitive << clk;
		sel = 3;
	}

	void processing(){
		int v1 = rand() % 7;
		sel = v1;
		//int v2 =( rand() % 5 )+1;
		op_sel.write(sel);

	}

};


SC_MODULE(drain)
{
	sc_in<sc_bv<8>> stat;
	sc_in<sc_bv<32>> acc;
	sc_in<sc_bv<32>> data1;
	sc_in<sc_bv<32>> data2;
	sc_clock clk;

SC_CTOR(drain):clk("clk", sc_time(1, SC_SEC))
	{
	SC_METHOD(processing);
	sensitive << acc << stat << data1 << data2 ;
	dont_initialize();
	}

	void processing()
	{
		//cout << "Time: " << sc_time_stamp() << endl;
		cout << "  ACC: " <<  acc.read() << endl;


	}

};
