#ifndef SIMPLE_SERVER_MAIN_SERVER_H
#define SIMPLE_SERVER_MAIN_SERVER_H
#include <open62541.h>

#if defined(__cplusplus)
extern "C" {
#endif

extern UA_NodeId Detector_id;
extern UA_NodeId Detector_value_id;

void register_objects(UA_Server *server);

#if defined(__cplusplus)
}
#endif

#endif //SIMPLE_SERVER_MAIN_SERVER_H
