#ifndef _main_hpp_
#define _main_hpp_

#include <Arduino.h>
#include "ground/str/str.hpp"

using namespace gf;

void demo();

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    while(!Serial) { delay(0.01); }
    Serial.println();
    Serial.println();

    demo();
}
void loop() {}

#endif
