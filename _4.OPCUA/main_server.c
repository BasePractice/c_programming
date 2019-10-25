#include "main_server.h"
#include <open62541.h>

UA_NodeId Detector_id;
UA_NodeId Detector_value_id;
static UA_UInt16 detector_value = 0;

void write_detector_value(UA_UInt16 value);

static UA_StatusCode
read_variable(UA_Server *server,
              const UA_NodeId *session_id, void *session_context,
              const UA_NodeId *node_id, void *node_context,
              UA_Boolean source_time_stamp, const UA_NumericRange *range,
              UA_DataValue *data) {
    UA_Variant_setScalarCopy(&data->value, &detector_value, &UA_TYPES[UA_TYPES_UINT16]);
    data->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
write_variable(UA_Server *server,
               const UA_NodeId *session_id, void *session_context,
               const UA_NodeId *node_id, void *node_context,
               const UA_NumericRange *range, const UA_DataValue *data) {
    if (data->value.type == &UA_TYPES[UA_TYPES_UINT16]) {
        detector_value = *(UA_UInt16 *) data->value.data;
        write_detector_value(detector_value);
    } else {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Can't write variable. Bad type");
    }
    return UA_STATUSCODE_GOOD;
}


void register_objects(UA_Server *server) {
    UA_StatusCode sc;
    {
        UA_ObjectAttributes device_attributes = UA_ObjectAttributes_default;
        device_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Устройство");
        sc = UA_Server_addObjectNode(server, UA_NODEID_NULL,
                                     UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                     UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                     UA_QUALIFIEDNAME(1, "Device"),
                                     UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                     device_attributes, NULL, &Detector_id);
        if (sc != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Registration object error %s",
                         UA_StatusCode_name(sc));
        }
    }
    {
        UA_VariableAttributes type_attributes = UA_VariableAttributes_default;
        type_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Датчики");
        type_attributes.valueRank = UA_VALUERANK_SCALAR;
        type_attributes.dataType = UA_TYPES[UA_TYPES_UINT16].typeId;
        type_attributes.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        type_attributes.writeMask = UA_WRITEMASK_DISPLAYNAME | UA_WRITEMASK_DESCRIPTION;
        type_attributes.userWriteMask = UA_WRITEMASK_DISPLAYNAME | UA_WRITEMASK_DESCRIPTION;

#if 0
        sc = UA_Server_addVariableNode(server, UA_NODEID_NULL, Detector_id,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                       UA_QUALIFIEDNAME(1, "Device.Detector"),
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), type_attributes, NULL,
                                       &Detector_value_id);
#else
        UA_DataSource detector_data_source;
        detector_data_source.read = read_variable;
        detector_data_source.write = write_variable;
        sc = UA_Server_addDataSourceVariableNode(server, UA_NODEID_NULL, Detector_id,
                                                 UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                                 UA_QUALIFIEDNAME(1, "Device.Detector"),
                                                 UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), type_attributes,
                                                 detector_data_source, NULL, &Detector_value_id);
#endif
        if (sc != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Registration variable error %s",
                         UA_StatusCode_name(sc));
        }
    }
}
