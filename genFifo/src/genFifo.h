#include <systemc.h>
#include <vector>

template<class T>
class Ififo_read {
public:

	virtual T read_nb() = 0;
	virtual T read_b() = 0;
	virtual void reset() = 0;
};

class Ififo_write {

	virtual void write_nb(T data) = 0;
	virtual void write_b(T data) = 0;
	virtual void reset() = 0;
};


template<class T>
class genfifo :public sc_module,  public Ififo_write, public Ififo_read
{

	sc_clock clk;
	std::vector<T> vec;
	int num;



	void write_nb(T data);

	T read_nb();

	int get_ndata();

	void write_b(T data);

	T read_b();

	void reset();

	SC_CTOR(genFifo)//:clk("clk", sc_time(1, SC_SEC))
	{
		SC_THREAD(write_nb);
		SC_THREAD(read_nb);
		SC_METHOD(write_b);
		SC_METHOD(read_b);
		SC_METHOD(reset);
		SC_THREAD(get_ndata);
		sensitive << clk;

		num = 0;

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


SC_MODULE(producer)
{


	SC_CTOR(producer)
	{

	}

}

SC_MODULE(consumer)
{
	sc_in<int> in;

	SC_HAS_PROCESS(consumer);
	consumer(sc_module_name n)
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
