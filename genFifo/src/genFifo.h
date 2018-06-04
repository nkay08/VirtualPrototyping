#include <systemc.h>
#include <vector>

template<class T>
class IGenFifo_read : virtual public sc_interface{
public:

	virtual T read_nb() = 0;
	virtual T read_b() = 0;
	virtual void reset() = 0;
};

template<class T>
class IGenFifo_write : virtual public sc_interface{
public:
	virtual void write_nb(T data) = 0;
	virtual void write_b(T data) = 0;
	virtual void reset() = 0;
};


template<class T>
class genfifo :public sc_module, public IGenFifo_read<T>, public IGenFifo_write<T>
{

private:
	sc_clock clk;
	std::vector<T> vec;
	int num;
	sc_event write_event, read_event, write_begin, write_end, read_begin, read_end;
	int write, read;
	int max = 50;

public:
	void write_nb(T data);

	T read_nb();

	int get_ndata();

	void write_b(T data);

	T read_b();

	void reset();

	SC_CTOR(genFifo)//:clk("clk", sc_time(1, SC_SEC))
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


SC_MODULE(producer)
{
	//sc_out<int>;
	bool run;
	bool random;
	IGenFifo_write<int> genFifo;

	SC_CTOR(producer)
	{
		SC_THREAD(processing);
		random = false;
	}
	void processing(){
		while(run){
			if(random){
				int v1 = rand() % 100;
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

}

SC_MODULE(consumer)
{
	//sc_in<int> in;
	bool run;
	int val;
	bool random;
	IGenFifo_read<T>* genFifo;

	SC_CTOR(producer)
	{
		SC_THREAD(processing);
		//sensitive << in;
		//dont_initialize();

	}

	void processing()
	{
		while(run){
			if(random){
				int v1 = rand() % 100;
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
