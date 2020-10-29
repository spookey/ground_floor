#ifndef _test_hpp_
#define _test_hpp_

#include <Arduino.h>
#include <unity.h>

#include "ground/str/str.hpp"

using namespace gf;

void run();

void setup() {
    UNITY_BEGIN();
    run();
    UNITY_END();
}

void loop() {}

#endif
