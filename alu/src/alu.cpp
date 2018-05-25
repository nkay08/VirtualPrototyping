#include <systemc.h>
#include "alu.h"



void alu::processing(){

switch(op_sel){

case "000": aluand();
	break;
case "001": aluor();
	break;
case "010": aluxor();
	break;
case "011": aluror();
	break;
case "100": alurol();
	break;
case "101":	aluadd();
	break;
case "110":	alumul();
	break;
case "111": aludiv;
	break;
}


}


void alu::aluand(){
sc_bv<32> result= (data1.read & data2.read());
acc.write(result);

}
void alu::aluor(){
sc_bv<32> result= (data1.read | data2.read());
acc.write(result);
}
void alu::aluxor(){
	sc_bv<32> result= (data1.read ^ data2.read());
	acc.write(result);

}
void alu::aluadd(){
int int1 = data1.read().to_int();
int int2 = data2.read().to_int();
int result = int1 + int2;

acc.write(sc_bv<32>(result));
if((acc.read() < data1.read()) || (acc.read() < data2.read())){
	//overflow

}

}

void alu::alumul(){
	int int1 = data1.read().to_int();
	int int2 = data2.read().to_int();
	int result = int1 + int2;
	sc_bv<32> max;
	max = "11111111111111111111111111111111";

	acc.write(sc_bv<32>(result));
	int result2 = acc.read().to_int();
	if(result >= max.to_int()){
		//overflow

	}

}
void alu::aludiv(){
	int int1 = data1.read().to_int();
	int int2 = data2.read().to_int();
	int result = int1 / int2;

	acc.write(sc_bv<32>(result));
	if((acc.read() > data1.read()) ){
		//underflow

	}
}
void alu::aluror(){

}
void alu::alurol(){

}
