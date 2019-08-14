//
// Created by nkay on 8/14/19.
//

#ifndef DCMOTOR_TARGET_H
#define DCMOTOR_TARGET_H

#include <systemc.h>
#include <tlm.h>
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "pid.h"

inline std::string getCurrentDateTime( std::string s ){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
};
inline void Logger( std::string logMsg ){

    std::string filePath = "./log_"+getCurrentDateTime("date")+".txt";
    std::string now = getCurrentDateTime("now");
    ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app );
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}

#define PID_CR_ADR 0x00
#define PID_CHER_ADR 0x04
#define PID_CHDR_ADR 0x14
#define PID_CHSR_ADR 0x18
#define PID_CHGR1_ADR 0x48
#define PID_CHGR2_ADR 0x4C
#define PID_CHGR3_ADR 0x50
#define SIZE 0x50

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
//            freopen("log.txt", "w", stdout);
//            cout << "message" << endl;
            Logger("------------------------------------------------------------------------------------------");
            Logger(sc_time_stamp().to_string());
            Logger("PID_CR: " + PID_CR.to_string());
            Logger("PID_CHSR: " + PID_CHSR.to_string());
            Logger("PID_CHER: " + PID_CHER.to_string());
            Logger("PID_CHDR: " + PID_CHDR.to_string());
            Logger("PID_CHGR1: " + PID_CHGR1.to_string());
            Logger("PID_CHGR2: " + PID_CHGR2.to_string());
            Logger("PID_CHGR3: " + PID_CHGR3.to_string());
            Logger("Real PID coefficents: K=" + std::to_string(pid1->p->Kp) + ", Ki=" + std::to_string(pid1->i->Ki) + ", Kd=" + std::to_string(pid1->d->Kd) );
            Logger("------------------------------------------------------------------------------------------");
        }

        void decide_action(){
            // start configuration
            if (PID_CR[1]){

                if (PID_CHSR[0])
                {
                    pid1->enableP(PID_CHGR1.to_int());
                }
                else
                {
                    pid1->disableP();
                }
                if (PID_CHSR[1])
                {
                    pid1->enableI(PID_CHGR2.to_int());
                }
                else
                {
                    pid1->disableI();
                }
                if (PID_CHSR[2])
                {
                    pid1->enableD(PID_CHGR3.to_int());
                }
                else
                {
                    pid1->disableD();
                }


            }
        }


        };

#endif //DCMOTOR_TARGET_H
