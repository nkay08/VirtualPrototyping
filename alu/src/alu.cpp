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

}
void alu::aluor(){

}
void alu::aluxor(){

}
void alu::aluadd(){

}

void alu::alumul(){

}
void alu::aludiv(){

}
void alu::aluror(){

}
void alu::alurol(){

}
