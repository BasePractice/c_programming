#include <signal.h>
#include "open62541.h"

#ifdef _MSC_VER
#pragma warning(disable:4996) // warning C4996: 'UA_Client_Subscriptions_addMonitoredEvent': was declared deprecated
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

static UA_Boolean running = true;

static void stop_handler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

#ifdef UA_ENABLE_SUBSCRIPTIONS

static void
handler_events(UA_Client *client, UA_UInt32 subId, void *sub_context,
               UA_UInt32 mon_id, void *mon_context,
               size_t n_event_fields, UA_Variant *event_fields) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Notification");

    /* The context should point to the mon_id on the stack */
    UA_assert(*(UA_UInt32 *) mon_context == mon_id);

    for (size_t i = 0; i < n_event_fields; ++i) {
        if (UA_Variant_hasScalarType(&event_fields[i], &UA_TYPES[UA_TYPES_UINT16])) {
            UA_UInt16 severity = *(UA_UInt16 *) event_fields[i].data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Severity: %u", severity);
        } else if (UA_Variant_hasScalarType(&event_fields[i], &UA_TYPES[UA_TYPES_LOCALIZEDTEXT])) {
            UA_LocalizedText *lt = (UA_LocalizedText *) event_fields[i].data;
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Message: '%.*s'", (int) lt->text.length, lt->text.data);
        } else {
#ifdef UA_ENABLE_TYPEDESCRIPTION
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Don't know how to handle type: '%s'", event_fields[i].type->typeName);
#else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                        "Don't know how to handle type, enable UA_ENABLE_TYPEDESCRIPTION "
                        "for typename");
#endif
        }
    }
}

const size_t n_select_clauses = 2;

static UA_SimpleAttributeOperand *
setup_select_clauses(void) {
    UA_SimpleAttributeOperand *select_clauses = (UA_SimpleAttributeOperand *)
            UA_Array_new(n_select_clauses, &UA_TYPES[UA_TYPES_SIMPLEATTRIBUTEOPERAND]);
    if (!select_clauses)
        return NULL;

    for (size_t i = 0; i < n_select_clauses; ++i) {
        UA_SimpleAttributeOperand_init(&select_clauses[i]);
    }

    select_clauses[0].typeDefinitionId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE);
    select_clauses[0].browsePathSize = 1;
    select_clauses[0].browsePath = (UA_QualifiedName *)
            UA_Array_new(select_clauses[0].browsePathSize, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
    if (!select_clauses[0].browsePath) {
        UA_SimpleAttributeOperand_delete(select_clauses);
        return NULL;
    }
    select_clauses[0].attributeId = UA_ATTRIBUTEID_VALUE;
    select_clauses[0].browsePath[0] = UA_QUALIFIEDNAME_ALLOC(0, "Message");

    select_clauses[1].typeDefinitionId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE);
    select_clauses[1].browsePathSize = 1;
    select_clauses[1].browsePath = (UA_QualifiedName *)
            UA_Array_new(select_clauses[1].browsePathSize, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
    if (!select_clauses[1].browsePath) {
        UA_SimpleAttributeOperand_delete(select_clauses);
        return NULL;
    }
    select_clauses[1].attributeId = UA_ATTRIBUTEID_VALUE;
    select_clauses[1].browsePath[0] = UA_QUALIFIEDNAME_ALLOC(0, "Severity");

    return select_clauses;
}

#endif

int register_client(const char *url) {
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);

    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    /* opc.tcp://uademo.prosysopc.com:53530/OPCUA/SimulationServer */
    /* opc.tcp://opcua.demo-this.com:51210/UA/SampleServer */
    UA_StatusCode ret = UA_Client_connect(client, url);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return EXIT_FAILURE;
    }

#ifdef UA_ENABLE_SUBSCRIPTIONS
    /* Create a subscription */
    UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
    UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
                                                                            NULL, NULL, NULL);
    if (response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_Client_disconnect(client);
        UA_Client_delete(client);
        return EXIT_FAILURE;
    }
    UA_UInt32 sub_id = response.subscriptionId;
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create subscription succeeded, id %u", sub_id);

    /* Add a MonitoredItem */
    UA_MonitoredItemCreateRequest item;
    UA_MonitoredItemCreateRequest_init(&item);
    item.itemToMonitor.nodeId = UA_NODEID_NUMERIC(0, 2253); // Root->Objects->Server
    item.itemToMonitor.attributeId = UA_ATTRIBUTEID_EVENTNOTIFIER;
    item.monitoringMode = UA_MONITORINGMODE_REPORTING;

    UA_EventFilter filter;
    UA_EventFilter_init(&filter);
    filter.selectClauses = setup_select_clauses();
    filter.selectClausesSize = n_select_clauses;

    item.requestedParameters.filter.encoding = UA_EXTENSIONOBJECT_DECODED;
    item.requestedParameters.filter.content.decoded.data = &filter;
    item.requestedParameters.filter.content.decoded.type = &UA_TYPES[UA_TYPES_EVENTFILTER];

    UA_UInt32 mon_id = 0;

    UA_MonitoredItemCreateResult result =
            UA_Client_MonitoredItems_createEvent(client, sub_id,
                                                 UA_TIMESTAMPSTORETURN_BOTH, item,
                                                 &mon_id, handler_events, NULL);

    if (result.statusCode != UA_STATUSCODE_GOOD) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Could not add the MonitoredItem with %s", UA_StatusCode_name(ret));
        goto cleanup;
    } else {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Monitoring 'Root->Objects->Server', id %u", response.subscriptionId);
    }

    mon_id = result.monitoredItemId;

    while (running)
        ret = UA_Client_run_iterate(client, 100);

    /* Delete the subscription */
    cleanup:
    UA_MonitoredItemCreateResult_clear(&result);
    UA_Client_Subscriptions_deleteSingle(client, response.subscriptionId);
    UA_Array_delete(filter.selectClauses, n_select_clauses, &UA_TYPES[UA_TYPES_SIMPLEATTRIBUTEOPERAND]);
#endif

    UA_Client_disconnect(client);
    UA_Client_delete(client);
    return ret == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}

UA_UInt16 read_value(UA_Client *client, UA_NodeId node_id) {
    UA_Variant value;
    UA_StatusCode ret;
    UA_UInt16 v = 0;

    UA_Variant_init(&value);
    ret = UA_Client_readValueAttribute(client, node_id, &value);
    if (ret == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_UINT16])) {
        v = *(UA_UInt16 *) value.data;
    } else {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Read value error: %s", UA_StatusCode_name(ret));
    }
    return v;
}

void write_value(UA_Client *client, UA_NodeId node_id, UA_UInt16 n) {
    UA_Variant value;

    UA_Variant_init(&value);
    UA_Variant_setScalar(&value, (void *) &n, &UA_TYPES[UA_TYPES_UINT16]);
    UA_StatusCode ret = UA_Client_writeValueAttribute(client, node_id, &value);

    if (ret == UA_STATUSCODE_GOOD) {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Value %d write success", n);
    } else {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Value %d write failure, error: %s", n,
                     UA_StatusCode_name(ret));
    }
}

/**
 * UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME)
 * UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
 * UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "date is: %u-%u-%u %u:%u:%u.%03u\n",
                    dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
 * */
UA_DateTime read_timestamp(UA_Client *client, const UA_NodeId node_id) {
    UA_DateTime timestamp = 0;
    UA_Variant value;
    UA_StatusCode ret;

    UA_Variant_init(&value);
    ret = UA_Client_readValueAttribute(client, node_id, &value);

    if (ret == UA_STATUSCODE_GOOD &&
        UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
        timestamp = *(UA_DateTime *) value.data;
    }
    UA_Variant_clear(&value);
    return timestamp;
}
