#ifndef _test_hpp_
#define _test_hpp_

#include <Arduino.h>
#include <unity.h>

#include "ground/str/str.hpp"

#define TLEN     8      // tiny
#define SLEN    16      // small

using namespace gf;

void run();

void setup() {
    UNITY_BEGIN();
    run();
    UNITY_END();
}

void loop() {}

#endif
