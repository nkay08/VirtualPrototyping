#include <systemc.h>




SCA_TDF_MODULE(proportional){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den, s;
	sca_tdf::sca_ltf_nd ltf;

	float Kp;

	void initialize(){
		num(0)=Kp;
		den(0)=1;
	}

	void setKp(float sKp){
		Kp = sKp;
	}

	void processing(){
		//out.write( in.read() * Kp);
		float temp = ltf( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(proportional):Kp(1){

	}
};


SCA_TDF_MODULE(integral){

	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den;
	sca_tdf::sca_ltf_nd ltf;
	sca_util::sca_vector<float> s;
	double Ki;

	sca_tdf::sca_ltf_nd pid_integral;

	void initialize(){
		num(0) = Ki;
		den(0) = 1.0 ;
	}

	void setKi(float sKi){
		Ki = sKi;
	}

	void processing(){
		float infloat = in.read();
		out.write( ltf( num,den,in.read() ) );
	}

	SCA_CTOR(integral):Ki(4*M_PI){

	}
};


SCA_TDF_MODULE(derivative){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_util::sca_vector<float>	num, den, s;
	double Kp;

	sca_tdf::sca_ltf_nd pid_derivative;

	void initialize(){

	}

	void setKd(float sKd){
		Kd = sKd;
	}

	void processing(){
		float temp = pid_derivative( num,den,in.read() );
		out.write(temp);
	}

	SCA_CTOR(derivative):Kp(1/15.0){

	}
};

SCA_TDF_MODULE(diff){
	sca_tdf::sca_in<float> in_ref;
	sca_tdf::sca_in<float> in_meas;
	sca_tdf::sca_out<float> out;


	SCA_CTOR(diff){

	}

	void initialize(){

	}
	void processing(){
			out.write( in_want.read() - in_meas.read() );
	}

}

SCA_TDF_MODULE(pidsum){
	sca_tdf::sca_in<float> in_p;
	sca_tdf::sca_in<float> in_i;
	sca_tdf::sca_in<float> in_d;
	sca_tdf::sca_out<float> out;


	SCA_CTOR(pidsum){

	}

	void initialize(){

	}
	void processing(){
		out.write( in_p.read() + in_i.read() + in_d.read() );

	}

}

SCA_TDF_MODULE(pid){
	sca_tdf::sca_in<float> in_ref;
	sca_tdf::sca_in<float> in_meas;
	sca_tdf::sca_out<float> out;
	sca_tdf::sca_signal<float> diff2pid;
	sca_tdf::sca_signal<float> p2sum;
	sca_tdf::sca_signal<float> i2sum;
	sca_tdf::sca_signal<float> d2sum;

	proportional* p;
	integral* i;
	derivative* d;
	diff* diff1;
	pidsum* pidsum1;

	void initialize(){

	}

	void processing(){



	}

	SCA_CTOR(pid):p("p"),i("i"),d("d"),diff1("diff"),pidsum1("pidsum"){
		diff1->in_ref(in_ref);
		diff1->in_meas(in_meas);
		diff1->out(diff2pid);
		p->in(diff2pid);
		p->out(p2sum);
		i->in(diff2pid);
		i->out(i2sum);
		d->in(diff2pid);
		d->out(d2sum);
		pidsum1->p_in(p2sum);
		pidsum1->i_in(i2sum);
		pidsum1->d_in(d2sum);
		pidsum1->out(out);

	}

	void setKp(float sKp){
		p->setKp(sKp);
	}

	void setKi(float sKi){
		i->setKi(sKi);
	}

	void setKd(float sKd){
		d->setKd(sKd);
	}
};


SCA_TDF_MODULE(pwm){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;


	SCA_CTOR(pwm){

	}

	void initialize(){

	}

	void processing(){

	}

}

SCA_TDF_MODULE(dcmotor){
	sca_tdf::sca_in<float> in;
	sca_tdf::sca_in<float> out;
	sca_tdf::sca_ltf_nd ltf;
	sc_
	// sc_clock clk;


	void initialize(){

	}


	void processing(){

	}

	SCA_CTOR(dcmotor)//:clk("clk", sc_time(1, SC_SEC))
	{

	}




};



SC_MODULE(drain)
{
	sc_in<int> in;

	SC_CTOR(drain)
	{
		SC_METHOD(processing);
		sensitive << clk;
	}

	/*
	SC_HAS_PROCESS(drain);
	drain(sc_module_name n)
	{
		SC_METHOD(processing);
		sensitive << in;
		dont_initialize();

	}
	 */

	void processing()
	{
		cout << "Time: " << sc_time_stamp() << endl;
		cout << "Counter value: " <<  in.read();
		cout << endl;

	}

};
