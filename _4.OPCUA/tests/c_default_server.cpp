#include <cstdlib>
#include <cstring>
#include <open62541.h>
#include "default_server.h"
#include "catch.hpp"

#if defined(WIN32)

#include <windows.h>

#else
#include <unistd.h>
#endif

#define DEFAULT_SERVER_TAG "[opcua/default/server]"

TEST_CASE("Проверка записи значения", DEFAULT_SERVER_TAG) {
    opcua::default_client c;

    c.write_value(10);
    UA_UInt16 v = c.read_value();
    REQUIRE(v == 10);
}
