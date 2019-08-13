//
// Created by nkay on 8/13/19.
//

#ifndef SBITCOUNTER_DCMOTOR_TLM_H
#define SBITCOUNTER_DCMOTOR_TLM_H

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

//        dcmotor_initiator(sc_module name, sc_time time_delay=sc_time(50, SC_NS))
        SC_CTOR(dcmotor_initiator)
        : socket("socket")
        {
            delay = sc_time(50, SC_NS);
            SC_THREAD(thread_process);
//            socket.register_nb_transport_bw(this, &dcmotor_initiator::nb_transport_bw);
        }

        virtual void nb_transport_bw(tlm::tlm_generic_payload& trans, sc_time& delay )
        {

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
            tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
            int adr = 0;
            int test_num = 1;

            // reset pid
            cout << "start test " << test_num << endl;
            adr = PID_CR_ADR;
            data_bv = "01";

            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            cout << "done test " << test_num << endl;
            test_num ++;

            wait(delay);

            // read P
            cout << "start test " << test_num << endl;
            adr = PID_CHGR1_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            sc_bv<32> temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);

            // read I
            cout << "start test " << test_num << endl;
            adr = PID_CHGR2_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);

            // read D
            cout << "start test " << test_num << endl;
            adr = PID_CHGR3_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // try write P, but configuration not started
            cout << "start test " << test_num << endl;
            adr = PID_CHGR1_ADR;
            data_bv = 5;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // read P again should still be 0
            cout << "start test " << test_num << endl;
            adr = PID_CHGR1_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // start config
            cout << "start test " << test_num << endl;
            adr = PID_CR_ADR;
            data_bv = "10";
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // try write P, config started
            cout << "start test " << test_num << endl;
            adr = PID_CHGR1_ADR;
            data_bv = 5;
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // read P again should still be 0
            cout << "start test " << test_num << endl;
            adr = PID_CHGR1_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 5);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // read status for P, shoud be disabled
            cout << "start test " << test_num << endl;
            adr = PID_CHSR_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data.to_int() == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);

            // enable P
            cout << "start test " << test_num << endl;
            adr = PID_CHER_ADR;
            data_bv = "001";
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);

            // read status for P, shoud be enabled
            cout << "start test " << test_num << endl;
            adr = PID_CHSR_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data[0] == 1);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


            // disable P
            cout << "start test " << test_num << endl;
            adr = PID_CHDR_ADR;
            data_bv = "001";
            send_transaction(trans, tlm::TLM_WRITE_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);

            // read status for P, shoud be disabled
            cout << "start test " << test_num << endl;
            adr = PID_CHSR_ADR;
            data_bv = "0";
            send_transaction(trans, tlm::TLM_READ_COMMAND, data_bv.to_int(), adr);

            assert(trans->get_response_status() == tlm::TLM_OK_RESPONSE);
            temp_data = *(trans->get_data_ptr());
            assert(temp_data[0] == 0);
            cout << "done test " << test_num << endl;
            test_num ++;
            wait(delay);


        }

};



SC_MODULE(dcmotor_target)
        {
            pid* pid1;
        int memsize = 80 + 32;
//    int* mem;
//    sc_bv<memsize> mem = "0";
        sc_bv<8> PID_CR = "0";
        sc_bv<32> PID_CHER = "0";
        sc_bv<32> PID_CHDR = "0";
        sc_bv<32> PID_CHSR = "0";
        sc_bv<32> PID_CHGR1 = "0";
        sc_bv<32> PID_CHGR2 = "0";
        sc_bv<32> PID_CHGR3 = "0";

        int base_adr = 0x40038000;

        tlm_utils::simple_target_socket<dcmotor_target> socket;

//        dcmotor_target(sc_module name, int memsize_ = 0)
        SC_CTOR(dcmotor_target)
        : socket("socket")
        {
            socket.register_b_transport(this, &dcmotor_target::b_transport);

//        if (memsize_ > 0)
//        {
//            memsize_= memsize_;
//        }
//        mem = new int[memsize];
//        for (int i = 0; i < memsize; i++)
//        {
//            mem[i] = 0;
//            //randomized data
////            mem[i] = 0xAA000000 | (rand() % 256);
//        }


        }


        virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
        {
            tlm::tlm_command cmd = trans.get_command();
            sc_dt::uint64    adr = trans.get_address();
            unsigned char*   ptr = trans.get_data_ptr();
            unsigned int     len = trans.get_data_length();
            unsigned char*   byt = trans.get_byte_enable_ptr();
            unsigned int     wid = trans.get_streaming_width();

            if (adr >= sc_dt::uint64(memsize) || byt != 0 || len > 32 || wid < len)
                SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");

            if (adr + len > memsize)
//            if (false)
            {
                trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            }
            else
            {
                if ( cmd == tlm::TLM_READ_COMMAND )
                {


//                memcpy(ptr, &mem[adr], len);

                    int data = 5;
                    switch (adr)
                    {
                        case PID_CHSR_ADR:
                        {
                            data = PID_CHSR.to_int();
                            break;
                        }

                        case PID_CHGR1_ADR:
                        {
                            data = PID_CHGR1.to_int();
                            break;
                        }

                        case PID_CHGR2_ADR:
                        {
                            data = PID_CHGR2.to_int();
                            break;
                        }

                        case PID_CHGR3_ADR:
                        {
                            data = PID_CHGR3.to_int();
                            break;
                        }

                        default:
                        {
                            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                            break;
                        }
                    }

                    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));

                }
                else if ( cmd == tlm::TLM_WRITE_COMMAND )
                {

//                memcpy(&mem[adr], ptr, len);
                    switch (adr)
                    {
                        // start_configuration/ reset
                        case PID_CR_ADR :
                        {
                            sc_bv<8> temp = *ptr;
                            PID_CR = temp.to_int();
                            if (temp[0])
                            {
                                pid1->hard_reset();
                            }
                            break;
                        }
                            // enable channels
                        case PID_CHER_ADR :
                        {
                            // if configuration started
                            if (PID_CR[1]){

                                // set enabled register
                                sc_bv<32> temp = *ptr;
                                PID_CHER = temp.to_int();
                                // status = old_status_value OR new_enabled_value
                                sc_bv<32> res = (PID_CHSR | PID_CHER);
                                PID_CHSR = res.to_int();
                                decide_action();
                            }
                            break;
                        }

                            // disable channels
                        case PID_CHDR_ADR :
                        {
                            // if configuration started
                            if (PID_CR[1]) {
                                // set enabled register
                                sc_bv<32> temp = *ptr;
                                PID_CHDR = temp.to_int();
                                // status = old_status_value AND NOT new_disabled_value
                                sc_bv<32> res = (PID_CHSR & ~ PID_CHDR);
                                PID_CHSR = res.to_int();
                                decide_action();
                            }
                            break;
                        }
                        case PID_CHGR1_ADR :
                        {
                            // if configuration started
                            if (PID_CR[1]) {
                                // set Kp
                                sc_bv<32> temp = *ptr;
                                PID_CHGR1 = temp.to_int();
                                decide_action();
                            }
                            break;
                        }
                        case PID_CHGR2_ADR :
                        {
                            // if configuration started
                            if (PID_CR[1]) {
                                // set Ki
                                sc_bv<32> temp = *ptr;
                                PID_CHGR2 = temp.to_int();
                                decide_action();
                            }
                            break;
                        }
                        case PID_CHGR3_ADR :
                        {
                            // if configuration started
                            if (PID_CR[1]) {
                                // set Kd
                                sc_bv<32> temp = *ptr;
                                PID_CHGR2 = temp.to_int();
                                decide_action();
                            }
                            break;
                        }

                        default :
                        {
                            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
                            break;
                        }

                    }
                }


                trans.set_response_status( tlm::TLM_OK_RESPONSE );
            }

        }

        void decide_action(){
            // start configuration
            if (PID_CR[1]){

                if (PID_CHSR[0])
                {
                    pid1->enableP(PID_CHGR1.to_int());
                }
                if (PID_CHSR[1])
                {
                    pid1->enableI(PID_CHGR2.to_int());
                }
                if (PID_CHSR[2])
                {
                    pid1->enableD(PID_CHGR3.to_int());
                }


            }
        }


        };

#endif //SBITCOUNTER_DCMOTOR_TLM_H
