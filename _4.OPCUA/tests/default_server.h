#ifndef SIMPLE_SERVER_DEFAULT_SERVER_H
#define SIMPLE_SERVER_DEFAULT_SERVER_H

#include <cstdlib>
#include <open62541.h>
#include <main_server.h>

extern "C" {
void write_value(UA_Client *client, UA_NodeId node_id, UA_UInt16 n);
UA_UInt16 read_value(UA_Client *client, UA_NodeId node_id);
void default_server_start();
void default_server_stop();
UA_Client *default_client_start();
void default_client_stop();
}

#define UA_CLIENT_CONNECTION_STRING "opc.tcp://localhost:4840"

namespace opcua {
    struct default_server {
        default_server() {
            default_server_start();
        }

        virtual ~default_server() {
            default_server_stop();
        }
    };

    struct default_client {
        default_client() : c(default_client_start()) {};

        void write_value(UA_Int16 n) {
            ::write_value(c, Detector_value_id, n);
        }

        UA_UInt16 read_value() const {
            return ::read_value(c, Detector_value_id);
        }

        ~default_client() {
            default_client_stop();
        }

    private:
        UA_Client *c;
    };
}


#endif //SIMPLE_SERVER_DEFAULT_SERVER_H
