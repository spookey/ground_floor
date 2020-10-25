#ifndef _test_hpp_
#define _test_hpp_

#include <Arduino.h>
#include <unity.h>
#include "nothing.hpp"

void run();

void setup() {
    UNITY_BEGIN();
    run();
    UNITY_END();
}

void loop() {}

#endif
