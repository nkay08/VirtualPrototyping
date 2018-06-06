#include <systemc.h>
#include <vector>

template<class T>
class IGenFifo_read : virtual public sc_interface{
public:

	virtual T read_nb() = 0;
	virtual T read_b() = 0;

};

template<class T>
class IGenFifo_write : virtual public sc_interface{
public:
	virtual void write_nb( T data) = 0;
	virtual void write_b( T data) = 0;
	virtual void reset() = 0;
};


template<class T>
class genfifo :public sc_module, public IGenFifo_read<T>, public IGenFifo_write<T>
{

public:
	sc_clock clk;
	std::vector<T> vec;
	int num;
	sc_event write_event;
	sc_event read_event;
	sc_event write_begin;
	sc_event write_end;
	sc_event read_begin;
	sc_event read_end;
	int write, read;
	int max = 50;

private:
	void write_nb(T data);

	T read_nb();

	int get_ndata();

	void write_b( T data);

	T read_b();

	void reset();
public:
	SC_CTOR(genfifo)//:clk("clk", sc_time(1, SC_SEC))
	{
		//SC_THREAD(write_nb);
		//SC_THREAD(read_nb);
		//SC_METHOD(write_b);
		//SC_METHOD(read_b);
		//SC_METHOD(reset);
		//SC_THREAD(get_ndata);
		//sensitive << clk;

		num = 0;
		write = 0;
		read = 0;

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


SC_MODULE(intproducer)
{
	//sc_out<int>;
	bool run;
	bool random;

public:
	IGenFifo_write<int>* genFifo;
	SC_CTOR(intproducer)
	{
		SC_THREAD(processing);
		random = false;
		run= true;
	}
	void processing(){
		while(run){
			int v1 = rand() % 100;
			if(random){
				if(v1<30){
					genFifo->write_b(v1);
					cout << "Write_b value: " << v1 << endl;

				}
				else{
					genFifo->write_nb(v1);
					cout << "Write_nb value: " << v1 << endl;
				}

			}
			else{
				genFifo->write_nb(v1);
				cout << "Write_nb value: " << v1 << endl;
			}
		}
	}

};

SC_MODULE(intconsumer)
{
private:
	//sc_in<int> in;
	bool run;
	int val;
	bool random;

public:
	IGenFifo_read<int>* genFifo;
	SC_CTOR(intconsumer)
	{
		SC_THREAD(processing);
		random = false;
		run = true;
		//sensitive << in;
		//dont_initialize();

	}

	void processing()
	{
		while(run){
			int v1 = rand() % 100;
			if(random){

				if(v1 < 30) {
					val = genFifo->read_b();
					cout << "Read_b value: " << val << endl;
				}
				else{
					val = genFifo->read_nb();
					cout << "Read_nb value: " << val << endl;
				}
			}
			else{
				val = genFifo->read_nb();
				cout << "Read_nb value: " << val << endl;
			}
		}

	}

};
