//#include <systemc.h>
#include "alu.h"



void alu::processing(){
if(initonce){
	int v1 = rand() % 4294967296;
	int v2 = rand() % 4294967296;
	sc_bv<32> d1 = v1;
	sc_bv<32> d2 = v2;
	data1.write(d1);
	data2.write(d2);

	cout << "DATA1:  " << data1.read();
	cout << "    DATA2:  " << data2.read();
	cout << endl;

	//initonce = false;
	//cout << "Init for datas";
}

//cout << op_sel.read().to_int();
switch(op_sel.read().to_int()){

case 0:
	cout << "Operation  AND  ";
	aluand();
	break;
case 1:
	cout << "Operation  OR  ";
	aluor();
	break;
case 2:
	cout << "Operation  XOR  ";
	aluxor();
	break;
case 3:
	cout << "Operation  ROR  ";
	aluror();
	break;
case 4:
	cout << "Operation  ROL  ";
	alurol();
	break;
case 5:
	cout << "Operation  ADD  ";
	aluadd();
	break;
case 6:
	cout << "Operation  MUL  ";
	alumul();
	break;
case 7:
	cout << "Operation  DIV  ";
	aludiv();
	break;
}

//cout << "value: " <<  data1.read() << endl;
}


void alu::aluand(){
sc_bv<32> result= (data1.read() & data2.read());
acc.write(result);
sc_bv<8> st = 0;
stat.write(st);

}
void alu::aluor(){
sc_bv<32> result= (data1.read() | data2.read());
acc.write(result);
stat.write(0);
}
void alu::aluxor(){
	sc_bv<32> result= (data1.read() ^ data2.read());
	acc.write(result);
	sc_bv<8> st = 0;
	stat.write(st);
}
void alu::aluadd(){
int int1 = data1.read().to_int();
int int2 = data2.read().to_int();
int result = int1 + int2;

acc.write(sc_bv<32>(result));
sc_bv<8> st = 0;
if((acc.read().to_int() < data1.read().to_int()) || (acc.read().to_int() < data2.read().to_int())){
	//overflow
st = 1000;
}
else{
st = 0;
}
stat.write(st);
}

void alu::alumul(){
	int int1 = data1.read().to_int();
	int int2 = data2.read().to_int();
	int result = int1 + int2;
	sc_bv<32> max;
	max = "11111111111111111111111111111111";

	acc.write(sc_bv<32>(result));
	int result2 = acc.read().to_int();
	sc_bv<8> st = 0;
	if(result >= max.to_int()){
		//overflow
		st = 1000;

	}
	else{
	st = 0;
	}
	stat.write(st);
}
void alu::aludiv(){
	int int1 = data1.read().to_int();
	int int2 = data2.read().to_int();
	int result = int1 / int2;

	acc.write(sc_bv<32>(result));
	if((acc.read().to_int() > data1.read().to_int()) ){
		//underflow

	}
	sc_bv<8> st = 0;
	stat.write(st);
}
void alu::aluror(){
	sc_bv<32> temp;
	temp = data1.read();
	temp = temp.rrotate(1);
	acc.write(temp);
	/*
	sc_bv<32> temp;
	temp = data1.read();
	bool tempfirst = temp[0];
	for(int i = 0; i < temp.length() -1;i++){
		temp[i] = temp[i+1];


	}
	temp[temp.length()-1] = tempfirst;
	acc.write(temp);
	sc_bv<8> st = 0;
	stat.write(st);
	*/
}
void alu::alurol(){
	sc_bv<32> temp;
	temp = data1.read();
	temp = temp.lrotate(1);
	acc.write(temp);
	/*
	sc_bv<32> temp;
	temp = data1.read();
	int templast = temp[temp.length() -1];

	for(int i= 0; i< temp.length()-1;i++){
		temp[i+1] = temp[i];
	}
	temp[0]=templast;
	acc.write(temp);
	sc_bv<8> st = 0;
	stat.write(st);
	*/
}
