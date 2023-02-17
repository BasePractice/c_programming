#include <open62541/types.h>
#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>

void write_detector_value(UA_UInt16 value) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Writing %d value", value);
}

