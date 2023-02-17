#include <signal.h>
#include <stdlib.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/log_stdout.h>
#include "main_server.h"

UA_Boolean running = true;

static void stop_handler(int signal) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Received %d signal", signal);
    running = false;
}

int main(int argc, char **argv) {
    UA_StatusCode ret;
    UA_Server *server;

    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);
    signal(SIGKILL, stop_handler);

    server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setDefault(config);
//    config->logger = UA_Log_Stdout_withLevel(UA_LOGLEVEL_DEBUG);
    register_objects(server);
    ret = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return ret == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
