#include <systemc.h>
#include <vector>

template<class T>
class IGenFifo_read : virtual public sc_interface{
public:

	virtual bool read_nb(T &val) = 0;
	virtual void read_b(T &val) = 0;

};

template<class T>
class IGenFifo_write : virtual public sc_interface{
public:
	virtual bool write_nb( T data) = 0;
	virtual void write_b( T data) = 0;
	virtual void reset() = 0;
};


template<class T>
class genfifo : public IGenFifo_read<T>, public IGenFifo_write<T>
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
	int max;
	/*
private:

	void write_nb(T data);

	T read_nb();

	int get_ndata();

	void write_b( T data);

	T read_b();

	void reset();
	 */
public:
	//SC_CTOR(genfifo)//:clk("clk", sc_time(1, SC_SEC))
	genfifo(int max)
{
		//SC_THREAD(write_nb);
		//SC_THREAD(read_nb);
		//SC_THREAD(write_b);
		//SC_THREAD(read_b);
		//SC_THREAD(reset);
		//SC_THREAD(get_ndata);
		//sensitive << clk;


		write = 0;
		read = 0;
		this->max = max;
		vec = {1};

		vec.pop_back();
		num = vec.size();

}



private:

	bool write_nb(T value){

		if(this->num>=this->max){

			return false;
			cout << "waiting for read_end" << endl;
			wait(this->read_end);
		}
		this->write++;
		this->write_begin.notify(sc_core::SC_ZERO_TIME);


		this->vec.insert(this->vec.begin(),value);
		this->num++;

		this->write--;
		this->write_end.notify(sc_core::SC_ZERO_TIME);

		return true;
	}

	bool read_nb(T &val){

		if(this->num<=1){
			return false;
			cout << "waiting for write_end" << endl;
			wait(this->write_end);
		}
		this->read++;
		this->read_begin.notify(sc_core::SC_ZERO_TIME);


		T data = (this->vec.back());
		this->vec.pop_back();
		this->num--;

		this->read--;
		this->read_end.notify(sc_core::SC_ZERO_TIME);

		val = data;
		return true;

	}
	int get_ndata(){

		return num;
	}
	void write_b( T value){

		while(this->write>0 && this->read>0){
			cout << "Write_b blocked" << endl;
			wait(this->write_end | this->read_end);
		}
		while(num>=max){
			cout << "Buffer full. waiting for new read" << endl;
			wait(this->read_end);
		}


		this->write++;
		this->write_begin.notify(sc_core::SC_ZERO_TIME);


		this->vec.insert(vec.begin(),value);
		this->num++;

		this->write--;
		this->write_end.notify(sc_core::SC_ZERO_TIME);


	}

	void read_b(T &val){

		while(this->write>0 && this->read>0 ){
			cout << "Read_b blocked" << endl;
			wait(this->write_end | this->read_end);
		}
		while(num<1){
			cout << "Buffer empty. waiting for new write" << endl;
			wait(this->write_end);
		}
		this->read++;
		this->read_begin.notify(sc_core::SC_ZERO_TIME);


		T data = (this->vec.back());
		this->vec.pop_back();
		this->num--;

		this->read--;
		this->read_end.notify(sc_core::SC_ZERO_TIME);

		val = data;


	}

	void reset(){

		std::vector<T> newvec;
		this->vec = newvec;
		this->num = 0;

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

	sc_clock clk;
	//sc_out<int>;
	bool run;
	bool random;
	bool once;

public:
	sc_inout<int> data;
	sc_inout<bool> block;
	sc_inout<bool> succio;
	sc_inout<bool> startaction;

	IGenFifo_write<int>* genFifo;
	SC_CTOR(intproducer) :clk("clk", sc_time(2, SC_SEC))
	{
		SC_THREAD(processing);
		dont_initialize() ;
		sensitive << clk;
		random = false;
		run= true;
		once = true;
	}
	void setRandom(bool rand){
		this->random = rand;
	}
	void processing(){

		while(run){


			if(once){
				wait(15,SC_NS);
				once = false;
			}
			else{
			int v2 = rand() % 8;
			wait(v2,SC_NS);
			}
			int v1 = rand() % 100;
			if(random){
				if(v1<50){
					startaction.write(true);
					block.write(true);
					genFifo->write_b(v1);
					cout << "Write_b value: " << v1 << endl;
					data.write(v1);
					succio.write(true);
					startaction.write(false);

				}
				else{
					startaction.write(true);
					block.write(false);
					bool succ = genFifo->write_nb(v1);
					if (succ) {
						cout << "Write_nb value: " << v1 << endl;
						data.write(v1);
						succio.write(true);
					}
					else{
						succio.write(false);
					}
					startaction.write(false);
				}

			}
			else{
				startaction.write(true);
				block.write(false);
				bool succ = genFifo->write_nb(v1);
				if (succ) {
					cout << "Write_nb value: " << v1 << endl;
					data.write(v1);
					succio.write(true);

				}
				else {
					succio.write(false);
				}
				startaction.write(false);
			}
			//cout << "waiting 2" << endl;
			//wait(80, SC_NS);
			//wait(2, SC_SEC);
			//wait(2,SC_SEC);
			//cout << "waited 2" << endl;
		}

	}


};

SC_MODULE(intconsumer)
{

private:

	sc_clock clk;
	//sc_in<int> in;
	bool run;
	int val;
	bool random;

public:
	sc_inout<int> data;
	sc_inout<bool> block;
	sc_inout<bool> succio;
	sc_inout<bool> startaction;

	IGenFifo_read<int>* genFifo;
	SC_CTOR(intconsumer) :clk("clk", sc_time(1, SC_SEC))
	{
		SC_THREAD(processing);
		dont_initialize() ;
		random = false;
		run = true;
		//sensitive << in;
		//dont_initialize();
		sensitive << clk;

	}
	void setRandom(bool rand){
		this->random = rand;
	}

	void processing()
	{
		while(run){
			int v2 = rand() % 5;
			wait(v2,SC_NS);
			int v1 = rand() % 100;
			if(random){

				if(v1 < 50) {
					int val = -1;
					startaction.write(true);
					block.write(true);
					genFifo->read_b(val);
					cout << "Read_b value: " << val << endl;
					data.write(val);
					succio.write(true);
					startaction.write(false);
				}
				else{
					int val = -1;
					startaction.write(true);
					block.write(false);
					bool succ = genFifo->read_nb(val);
					if(succ){
						cout << "Read_nb value: " << val << endl;
						data.write(val);

						succio.write(true);
					}
					else{
						succio.write(false);
					}
					startaction.write(false);
				}
			}
			else{
				int val = -1;
				block.write(false);
				startaction.write(true);
				bool succ = genFifo->read_nb(val);

				if(succ){
					cout << "Read_nb value: " << val << endl;
					data.write(val);

					succio.write(true);
				}
				else {
					succio.write(false);
				}

				startaction.write(false);
			}
		}

	}

};
