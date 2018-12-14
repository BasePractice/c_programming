#include <catch2/catch.hpp>
#include "../10.Synchrinized/monitor.h"

void function_increment(void *object) {
    long *count = reinterpret_cast<long *>(object);
    (*count)++;
}

TEST_CASE("Инициализация монитора", "[Monitor]") {
    struct Monitor monitor{};
    auto *object = "p";

    SECTION("Инициализация") {
        monitor_init(&monitor, (void *)object);
        CHECK(monitor.object == object);
    }

    SECTION("Разрушение") {
        monitor_destroy(&monitor);
        REQUIRE(monitor.object == nullptr);
    }
}

TEST_CASE("Вызов monitor_enter", "[Monitor]") {
    struct Monitor monitor{};
    long object = 0L;


    monitor_init(&monitor, (void *)&object);
    CHECK(monitor.object == &object);
    monitor_enter(&monitor, function_increment);
    CHECK(object == 1);
    monitor_destroy(&monitor);
    REQUIRE(monitor.object == nullptr);
}


