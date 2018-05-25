/*
 * 17bitCounter.cpp
 *
 *  Created on: May 19, 2018
 *      Author: nkay
 */

#include <systemc.h>


void sbitCounter::processing(){

	//cout << "processing";
	if(reset.read()){
		cnt = "0";
	}

	if(count_en.read()){
		//cout << "counting";
		if(ud_ctrl.read()){
			//cout << "adding";
			add();
		}
		else {
			subtract();
		}

	}
	cnt_out.write(cnt);


	/*if(reset.read())cnt=0;
			if(count_en.read()){
				if(ud_ctrl.read()){
				/*
					if(cnt<max) {
						cnt++;
						ovf_intr.write(false);
					}
					else {
						cnt=0;
						ovf_intr.write(true);
					}
					cnt_out.write(cnt);
				/*
				}
				else {
					if(cnt>0){
						cnt--;
						unf_intr.write(false);
					}
					else{
						cnt=max;
						unf_intr.write(true);
					}
					cnt_out.write(cnt);

				}
			}
			/**/

}

void sbitCounter::add(){

	bool carry = true;
	///*
	for(int i = 0; i < cnt.length();++i){
		///*
		if(carry){
			if(cnt[i] == 0){
				cnt[i] = 1;
				carry = false;
			}
			else{
				cnt[i] = 0;
				carry = true;
			}
		}


	}
	/**/

	///*
	if(carry){
		ovf_intr.write(carry);
		cnt = "0";
	}

	/**/
	//cout << cnt << endl;
}

void sbitCounter::subtract(){
	bool carry = true;
	carry  = true;
	for(int i =0 ;i< cnt.length();++i){
		if(carry){
			if(cnt[i] == 1){
				cnt[i] = 0;
				carry = false;
			}
			else {
				cnt[i] = 1;
				carry = true;
			}
		}
	}
	if(carry){
		unf_intr.write(carry);
		cnt= "11111111111111111";
	}
}
