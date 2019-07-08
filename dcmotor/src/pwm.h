/*
 * pwm.h
 *
 *  Created on: Jul 18, 2018
 *      Author: nkay
 */

#ifndef DCMOTOR_SRC_PWM_H_
#define DCMOTOR_SRC_PWM_H_

#include <systemc.h>
#include "systemc-ams"

SCA_TDF_MODULE(pwm){
	sca_tdf::sca_in<double> in{"in"};
	sca_tdf::sca_out<double> out{"out"};



    double t_ramp;
    double t_duty;
	sc_core::sc_time t_step;
    double v_drv;
    double t_period;


    pwm(
            sc_core::sc_module_name nm,
            double v_drv_ = 1.0,
            const sca_core::sca_time& t_period_ = sca_core::sca_time(5.0, sc_core::SC_MS),
            const sca_core::sca_time& t_ramp_ = sca_core::sca_time(0.05, sc_core::SC_MS),
            const sca_core::sca_time& t_step_ = sca_core::sca_time(0.01, sc_core::SC_MS)
            )
	:in("in"),out("out"), t_ramp(t_ramp_.to_seconds()), v_drv(v_drv_), t_period(t_period_.to_seconds()), t_step(t_step_)
	{
	}

	void initialize(){

	}

	void processing(){
	    double t = get_time().to_seconds();
	    double t_pos = std::fmod( t, t_period );


	    double value = 0;
        float c = in.read();
        double t_duty_max = t_period - 2 * t_ramp;
        t_duty = c * t_duty_max;

        // check boundaries for t_duty
        if (t_pos < t_ramp) {
            if (t_duty < 0.0) {
                t_duty = 0.0;
            }
            if (t_duty > t_duty_max){
                t_duty = t_duty_max;
            }
        }


//        cout << "TIME: " << sc_time_stamp() << ", t_duty: " << t_duty << ", t_pos:" << t_pos << ", value: " << value << ", t_period: " << t_period << ", t: " << t <<  endl;

        if (t_pos < t_ramp) {
            value = (v_drv / t_ramp) * t_pos + 0;
        }
        else if (t_pos < t_ramp + t_duty) {
            value = (v_drv);
        }
        else if (t_pos < t_ramp + t_duty + t_ramp) {
            value = -1.0 * (v_drv / t_ramp) * (t_pos - t_ramp - t_duty) + v_drv;
        }
        else {
            value = 0;
        }

		out.write(value);

	}

	void set_attributes() {
        set_timestep( t_step );


		does_attribute_changes();
		accept_attribute_changes();
        }

	void change_attributes() {
		double t = get_time().to_seconds(); // current time
		double t_pos = std::fmod( t, t_period ); // time position inside pulse period
		// Calculate time step till next activation
		double dt = 0.0;
		if ( t_pos < t_ramp ) // rising edge

		    // activate once more middle of rising edge
            dt = t_pos < (t_ramp / 2) ? (t_ramp / 2) - t_pos : dt = t_ramp - t_pos;

		else if ( t_pos < t_ramp + t_duty ) // plateau
			dt = ( t_ramp + t_duty ) - t_pos;
		else if ( t_pos < t_ramp + t_duty + t_ramp ) // falling edge

		    // activate once more middle of falling edge
		    dt = ( t_pos < t_ramp + t_duty + (t_ramp / 2) ) ? ( t_ramp + t_duty + (t_ramp / 2) ) - t_pos : ( t_ramp + t_duty + t_ramp ) - t_pos;

		else // return to initial value
			dt = t_period - t_pos;

        t_step = sca_core::sca_time( dt, sc_core::SC_SEC );
		if ( t_step == sc_core::SC_ZERO_TIME ) // time step should advance
			t_step = sc_core::sc_get_time_resolution();

//		cout << "NEXT ACTIVATION: " << "t_step = " << t_step << ", t = " << t << ", t_pos = " << t_pos << ", t_period = " << t_period << ", counter = " << counter << endl;
		request_next_activation( t_step ); // request the next activation }
	}



};

SCA_TDF_MODULE(pwm_source){
        sca_tdf::sca_out<double> out;

    double value;

    void processing(){
        double t = get_time().to_seconds() * 1000;

        if  (t > 10) {
            value = 0.1;
        }
        if ( t > 20) {
            value = 1;
        }

        out.write(value);
//            cout << "Time: " << sc_time_stamp() << endl;
//            cout << "IN Value: " <<  value;
    }

    void set_attributes(){
            accept_attribute_changes();
    }

    SCA_CTOR(pwm_source):value(0.7)
    {
    }
};


SCA_TDF_MODULE(pwm_drain) {
    sca_tdf::sca_in<double> in;

        void set_attributes(){
            accept_attribute_changes();
        }

  void processing() {
      double tmp = in.read();
//        cout << "Time: " << sc_time_stamp() << endl;
//        cout << "OUT Value: " <<  tmp;
  };

  SCA_CTOR(pwm_drain){
  }
};

#endif /* DCMOTOR_SRC_PWM_H_ */
