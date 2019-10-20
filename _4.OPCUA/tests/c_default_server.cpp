#include <cstdlib>
#include <cstring>
#include <open62541.h>
#include "catch.hpp"

#if defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#define DEFAULT_SERVER_TAG "[opcua/default/server]"

TEST_CASE("Работа с сервером", DEFAULT_SERVER_TAG) {

}
