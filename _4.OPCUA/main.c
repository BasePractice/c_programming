#include <signal.h>
#include <stdlib.h>
#include <open62541.h>
#include "main_server.h"

UA_Boolean running = true;
static void stop_handler(int signal) {
    running = false;
}

int main(int argc, char **argv) {
    UA_StatusCode ret;
    UA_Server    *server;

    signal(SIGINT,  stop_handler);
    signal(SIGTERM, stop_handler);

    server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));
    register_objects(server);
    ret = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return ret == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
