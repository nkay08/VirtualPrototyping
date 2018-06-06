#include <systemc.h>
#include "genFifo.h"





template<class T>
void genfifo<T>::write_nb(T value){
	if(this->num==this->max){
		wait(this->read_end);
	}
	this->write_begin.notify();
	this->write++;

	this->vec.insert(this->vec.begin(),value);
	this->num++;

	this->write_end.notify();
	this->write--;
}
template<class T>
 T genfifo<T>::read_nb(){
	if(this->num==0){
		wait(this->write_end);
	}
	this->read_begin.notify();
	this->read++;

	T data = this->vec.pop_back();
	this->num--;

	this->read_end.notify();
	this->read--;
	return data;

}
template<class T>
int genfifo<T>::get_ndata(){

	return num;
}
template<class T>
void genfifo<T>::write_b( T value){

	while(this->write>0 && this->read>0){
		cout << "Write_b blocked" << endl;
		wait(this->write_end | this->read_end);
	}

	this->write_begin.notify();
	this->write++;

	this->vec.insert(vec.begin(),value);
	this->num++;

	this->write_end.notify();
	this->write--;

}

template<class T>
T genfifo<T>::read_b(){

	while(this->write>0 && this->read>0){
		cout << "Read_b blocked" << endl;
		wait(this->write_end | this->read_end);
	}


	this->read_begin.notify();
	this->read++;

	T data = vec.pop_back();
	this->num--;

	this->read_end.notify();
	this->read--;
	return data;

}
template<class T>
void genfifo<T>::reset(){

	std::vector<T> newvec;
	this->vec = newvec;
	this->num = 0;

}
