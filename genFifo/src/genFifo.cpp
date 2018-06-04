#include <systemc.h>
#include "genFifo.h"





void genFifo::write_nb(T value){


	vec.push_back(value);
	num++;
}

T genFifo::read_nb(){

T data = vec.pop();
num--
return data;

}

int genFifo::get_ndata(){

	return num;
}

void genFifo::write_b(T value){

	vec.push_back(value);
	num++:

}

T genFifo::read_b(){

	T data = vec.pop();
	num--
	return data;

}

void genFifo::reset(){

}
