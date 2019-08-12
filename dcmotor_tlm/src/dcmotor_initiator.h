//
// Created by nkay on 8/12/19.
//

#ifndef SBITCOUNTER_DCMOTOR_INITIATOR_H
#define SBITCOUNTER_DCMOTOR_INITIATOR_H

#include <systemc.h>
#include <tlm.h>
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"


SC_MODULE(dcmotor_initiator){

    tlm_utils::simple_initiator_socket<dcmotor_initiator> socket;
    int data;

    SC_CTOR(dcmotor_initiator)
            : socket("socket")
    {
        SC_THREAD(thread_process);
    }

    void thread_process()
    {
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10, SC_NS);
        int adr = 0;


        bool run = true;

        while (run)
        {
            tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);

            if (cmd == tlm::TLM_WRITE_COMMAND)
            {

            }
            else if (cmd == tlm::TLM_READ_COMMAND)
            {

            }

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

            cout << "trans = { " << (cmd ? 'W' : 'R') << ", " << hex << i
                 << " } , data = " << hex << data << " at time " << sc_time_stamp()
                 << " delay = " << delay << endl;

            wait(delay);
        }


    }

};


#endif //SBITCOUNTER_DCMOTOR_INITIATOR_H
