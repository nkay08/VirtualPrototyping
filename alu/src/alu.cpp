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
case "111": aludiv();
	break;
}


}


void alu::aluand(){
sc_bv<32> result= (data1.read & data2.read());
acc.write(result);
stat.write("1");

}
void alu::aluor(){
sc_bv<32> result= (data1.read | data2.read());
acc.write(result);
stat.write("1");
}
void alu::aluxor(){
	sc_bv<32> result= (data1.read ^ data2.read());
	acc.write(result);
	stat.write("1");
}
void alu::aluadd(){
int int1 = data1.read().to_int();
int int2 = data2.read().to_int();
int result = int1 + int2;

acc.write(sc_bv<32>(result));
if((acc.read() < data1.read()) || (acc.read() < data2.read())){
	//overflow

}
stat.write("1");
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
	stat.write("1");
}
void alu::aludiv(){
	int int1 = data1.read().to_int();
	int int2 = data2.read().to_int();
	int result = int1 / int2;

	acc.write(sc_bv<32>(result));
	if((acc.read() > data1.read()) ){
		//underflow

	}
	stat.write("1");
}
void alu::aluror(){
	sc_bv<32> temp;
	temp = data1.read();
	int tempfirst = temp[0];
	for(int i = 0; i < temp.length() -1;i++){
		temp[i] = temp[i+1];

	}
	temp[temp.length()-1] = tempfirst;
	acc.write(temp);
	stat.write("1");
}
void alu::alurol(){
	sc_bv<32> temp;
	temp = data1.read();
	int templast = temp[temp.length-1];

	for(int i= 0; i< temp.length()-1;i++){
		temp[i+1] = temp[i];
	}
	temp[0]=templast;
	acc.write(temp);
	stat.write("1");
}
