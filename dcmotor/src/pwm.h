/*
 * pwm.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PWM_H_
#define DCMOTOR_SRC_PWM_H_



SCA_TDF_MODULE(pwm){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;


private:
	double t_ramp;
	double t_duty;
	double t_step;

public:


	SCA_CTOR(pwm):in("in"),out("out"),t_ramp(0.05),t_step(5){

	}

	void initialize(){

	}

	void processing(){
		float c = in.read();
		t_duty = c * ( t_step - 2 * t_ramp  );
	}

	void set_attributes() {
		allow_dynamic_tdf();
	}

	void change_attributes() {
		double t = get_time().to_seconds(); // current time
		double t_pos = std::fmod( t, t_period ); // time position inside pulse period
		// Calculate time step till next activation
		double dt = 0.0;
		if ( t_pos < t_ramp ) // rising edge
			dt = t_ramp - t_pos;
		else if ( t_pos < t_ramp + t_duty ) // plateau
			dt = ( t_ramp + t_duty ) - t_pos;
		else if ( t_pos < t_ramp + t_duty + t_ramp ) // falling edge
			dt = ( t_ramp + t_duty + t_ramp ) - t_pos;
		else // return to initial value
			dt = t_period - t_pos; t_step = sca_core::sca_time( dt, sc_core::SC_SEC );
		if ( t_step == sc_core::SC_ZERO_TIME ) // time step should advance
			t_step = sc_core::sc_get_time_resolution();
		request_next_activation( t_step ); // request the next activation }
	}



};


#endif /* DCMOTOR_SRC_PWM_H_ */
