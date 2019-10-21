#include <thread>
#include <open62541.h>
#include <main_server.h>
#include "default_server.h"

volatile UA_Boolean _default_server_running = false;
UA_Server *_default_server;

static void *monitoring_loop(void *args) {
    UA_ServerConfig *config = UA_Server_getConfig(_default_server);
    UA_ServerConfig_setMinimal(config, 4840, nullptr);
    register_objects(_default_server);
    _default_server_running = true;
    if (UA_Server_run(_default_server, &_default_server_running) == UA_STATUSCODE_GOOD) {
        UA_sleep_ms(1500);
        UA_Server_delete(_default_server);
        _default_server = nullptr;
    }
    return nullptr;
}

void default_server_start() {
    _default_server = UA_Server_new();
    std::thread t([] {
        monitoring_loop(nullptr);
    });
    t.detach();
}

void default_server_stop() {
    _default_server_running = false;
    UA_LOG_DEBUG(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Server send stop signal");
}

volatile UA_Boolean _default_client_running = true;
UA_Client *_default_client = nullptr;

UA_Client *default_client_start() {
    _default_client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(_default_client));

    /* opc.tcp://uademo.prosysopc.com:53530/OPCUA/SimulationServer */
    /* opc.tcp://opcua.demo-this.com:51210/UA/SampleServer */
    UA_StatusCode ret = UA_Client_connect(_default_client, UA_CLIENT_CONNECTION_STRING);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_Client_delete(_default_client);
        return nullptr;
    }
//    std::thread s([] {
//        UA_StatusCode sc = UA_STATUSCODE_GOOD;
//        while (_default_client_running && sc == UA_STATUSCODE_GOOD)
//            sc = UA_Client_run_iterate(_default_client, 100);
//        UA_sleep_ms(1500);
//        UA_Client_disconnect(_default_client);
//        UA_Client_delete(_default_client);
//        _default_client = nullptr;
//    });
//    s.detach();
    return _default_client;
}

void default_client_stop() {
    _default_client_running = false;
    UA_Client_disconnect(_default_client);
    UA_Client_delete(_default_client);
    _default_client = nullptr;
}



