#include <systemc.h>
#include "genFifo.h"






void genFifo::write_nb(T value){
	if(num=max){
		wait(read_end);
	}
	write_begin.notify();
	write++;

	vec.insert(0,value);
	num++;

	write_end.notify();
	write--;
}

T genFifo::read_nb(){
	if(num=0){
		wait(write_end);
	}
	read_begin.notify();
	read++;

	T data = vec.pop_back();
	num--;

	read_end.notify();
	read--;
	return data;

}

int genFifo::get_ndata(){

	return num;
}

void genFifo::write_b(T value){

	while(write>0 && read>0){
		cout << "Write_b blocked" << endl;
		wait(write_end | read_end);
	}

	write_begin.notify();
	write++;

	vec.insert(0,value);
	num++;

	write_end.notify();
	write--;

}

T genFifo::read_b(){

	while(write>0 && read>0){
		cout << "Read_b blocked" << endl;
		wait(write_end | read_end);
	}


	read_begin.notify();
	read++;

	T data = vec.pop_back();
	num--;

	read_end.notify();
	read--;
	return data;

}

void genFifo::reset(){

	std::vector<T> newvec;
	vec = newvec;
	num = 0;

}
