//
// Created by nkay on 8/13/19.
//

#ifndef DCMOTOR_INITIATOR_H
#define DCMOTOR_INITIATOR_H

#include <systemc.h>
#include <tlm.h>
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "pid.h"

#define PID_CR_ADR 0x00
#define PID_CHER_ADR 0x04
#define PID_CHDR_ADR 0x14
#define PID_CHSR_ADR 0x18
#define PID_CHGR1_ADR 0x48
#define PID_CHGR2_ADR 0x4C
#define PID_CHGR3_ADR 0x50
#define SIZE 0x50



SC_MODULE(dcmotor_initiator){

        tlm_utils::simple_initiator_socket<dcmotor_initiator> socket;
        int data;
        sc_bv<32> data_bv = "0";

        sc_time delay;

        bool run_test = false;
        pid* pid1;

//        dcmotor_initiator(sc_module name, sc_time time_delay=sc_time(50, SC_NS))
        SC_CTOR(dcmotor_initiator)
        : socket("socket")
        {
            delay = sc_time(10, SC_MS);
            SC_THREAD(thread_process);
//            socket.register_nb_transport_bw(this, &dcmotor_initiator::nb_transport_bw);
        }

        virtual void nb_transport_bw(tlm::tlm_generic_payload& trans, sc_time& delay )
        {

        }

        std::tuple<tlm::tlm_generic_payload*, bool> pid_configure(bool enable= true)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "00";
            if (enable) temp[1] = 1;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CR_ADR);
            return std::make_tuple(trans, enable);
        }

        std::tuple<tlm::tlm_generic_payload*, bool> pid_reset(bool enable= true)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "00";
            if (enable) temp[0] = 1;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CR_ADR);
            return std::make_tuple(trans, enable);
        }

        std::tuple<tlm::tlm_generic_payload*, std::string> pid_status()
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, temp.to_int(), PID_CHSR_ADR);
            sc_bv<32> temp_data = *(trans->get_data_ptr());
            return std::make_tuple(trans, temp_data.to_string());
        }

        std::tuple<tlm::tlm_generic_payload*, bool> pid_enable(bool cgr1= false, bool cgr2= false, bool cgr3= false)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "000";
            if (cgr1) temp[0] = 1;
            if (cgr2) temp[1] = 1;
            if (cgr3) temp[2] = 1;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CHER_ADR);
            return std::make_tuple(trans, true);
        }

        std::tuple<tlm::tlm_generic_payload*, bool> pid_disable(bool cgr1= false, bool cgr2= false, bool cgr3= false)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "000";
            if (cgr1) temp[0] = 1;
            if (cgr2) temp[1] = 1;
            if (cgr3) temp[2] = 1;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CHDR_ADR);
            return std::make_tuple(trans, true);
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_gain1(int gain = 0)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = gain;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CHGR1_ADR);
            return std::make_tuple(trans, gain);
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_gain2(int gain = 0)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = gain;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CHGR2_ADR);
            return std::make_tuple(trans, gain);
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_gain3(int gain = 0)
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = gain;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, temp.to_int(), PID_CHGR3_ADR);
            return std::make_tuple(trans, gain);
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_get_gain1()
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, temp.to_int(), PID_CHGR1_ADR);
            sc_bv<32> temp_data = *(trans->get_data_ptr());
            return std::make_tuple(trans, temp_data.to_int());
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_get_gain2()
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, temp.to_int(), PID_CHGR2_ADR);
            sc_bv<32> temp_data = *(trans->get_data_ptr());
            return std::make_tuple(trans, temp_data.to_int());
        }

        std::tuple<tlm::tlm_generic_payload*, int> pid_get_gain3()
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_bv<32> temp = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, temp.to_int(), PID_CHGR3_ADR);
            sc_bv<32> temp_data = *(trans->get_data_ptr());
            return std::make_tuple(trans, temp_data.to_int());
        }

        void thread_process()
        {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_time delay = sc_time(10, SC_MS);
            int adr = 0;


            bool run = true;
            if (run_test)
            {
//                for (int i = 0; i <= 10; i++)
//                {
//                    cout << "run test "  <<  i << endl;
//                    test(i);
//                }
                    test();
            }
            else
            {
                while (run)
                {
                    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);

                    if (cmd == tlm::TLM_WRITE_COMMAND)
                    {

                    }
                    else if (cmd == tlm::TLM_READ_COMMAND)
                    {

                    }
                    data = data_bv.to_int();

                    trans->set_command( cmd );
                    trans->set_address( adr );
                    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
                    trans->set_data_length( 32 );
                    trans->set_streaming_width( 32 ); // = data_length to indicate no streaming
                    trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
                    trans->set_dmi_allowed( false ); // Mandatory initial value
                    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

                    socket->b_transport( *trans, delay );  // Blocking transport call

                    if ( trans->is_response_error() )
                        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

                    cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << adr
                         << " } , data = " << hex << data << " at time " << sc_time_stamp()
                         << " delay = " << delay << endl;

                    wait(delay);
                }
            }
        }

        void send_transaction(tlm::tlm_generic_payload *trans,tlm::tlm_command cmd, int new_data, int adr)
        {

            data = new_data;
            trans->set_data_length(32);
            trans->set_streaming_width(32); // = data_length to indicate no streaming
            trans->set_byte_enable_ptr(0); // 0 indicates unused
            trans->set_dmi_allowed(false); // Mandatory initial value
            trans->set_command(cmd);
            trans->set_address(adr);
            trans->set_data_ptr(reinterpret_cast<unsigned char *>(&data));
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE); // Mandatory initial value
            socket->b_transport(*trans, delay);  // Blocking transport call
            if (trans->is_response_error())
                SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

        }

        void test() {
//            tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
//            int adr = 0;
            int test_num = 1;

            {
                // reset pid
                cout << "start test " << test_num << endl;
                auto tple = pid_reset(true);
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->p->Kp == 0);
                assert(pid1->i->Ki == 0);
                assert(pid1->d->Kd == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // stop reset pid
                cout << "start test " << test_num << endl;

                auto tple = pid_reset(false);
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->p->Kp == 0);
                assert(pid1->i->Ki == 0);
                assert(pid1->d->Kd == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read P
                cout << "start test " << test_num << endl;
                auto tple = pid_get_gain1();
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read I
                cout << "start test " << test_num << endl;
                auto tple = pid_get_gain2();
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read D
                cout << "start test " << test_num << endl;
                auto tple = pid_get_gain3();
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // try write P, but configuration not started
                cout << "start test " << test_num << endl;
                auto tple = pid_gain1(5);
                auto ttrans = std::get<0>(tple);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read P again should still be 0
                cout << "start test " << test_num << endl;
                auto tple = pid_get_gain1();
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 0);
                assert(pid1->p->Kp == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // start config
                cout << "start test " << test_num << endl;
                auto tple = pid_configure(true);
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // try write P, config started
                cout << "start test " << test_num << endl;
                auto tple = pid_gain1(5);
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->p->Kp == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read P again should be value
                cout << "start test " << test_num << endl;
                auto tple = pid_get_gain1();
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 5);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read status for P, shoud be disabled
                cout << "start test " << test_num << endl;

                auto tple = pid_status();
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data.to_int() == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // enable P
                cout << "start test " << test_num << endl;
                auto tple = pid_enable(true, false, false);
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->p->Kp == 5);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read status for P, shoud be enabled
                cout << "start test " << test_num << endl;
                auto tple = pid_status();
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data[0] == 1);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // disable P
                cout << "start test " << test_num << endl;

                auto tple = pid_disable(true, false, false);
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                // read status for P, shoud be disabled
                cout << "start test " << test_num << endl;
                auto tple = pid_status();
                auto ttrans = std::get<0>(tple);

                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->p->Kp == 0);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data[0] == 0);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(delay);
            }

            {
                cout << "start test " << test_num << endl;
                pid_enable(false, true, false);
                wait(delay);
                pid_gain2(5);
                wait(delay);
                auto tple = pid_status();
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->i->Ki == 5);
                sc_bv<32> temp_data = *(ttrans->get_data_ptr());
                assert(temp_data[1] == 1);
                cout << "done test " << test_num << endl;
                test_num++;
                wait(sc_time(50, SC_MS));

            }

            {
                cout << "start test " << test_num << endl;
                auto tple = pid_reset(true);
                auto ttrans = std::get<0>(tple);
                assert(ttrans->get_response_status() == tlm::TLM_OK_RESPONSE);
                assert(pid1->i->Ki == 0);
                cout << "done test " << test_num << endl;
                test_num++;

            }

        }

};



#endif //DCMOTOR_INITIATOR_H
