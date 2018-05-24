/*
 * 17bitCounter.cpp
 *
 *  Created on: May 19, 2018
 *      Author: nkay
 */

#include <systemc.h>


void sbitCounter::processing(){
	/*if(reset.read())cnt=0;
			if(count_en.read()){
				if(ud_ctrl.read()){
				/**/
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
