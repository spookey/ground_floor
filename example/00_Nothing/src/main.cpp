#include "main.hpp"

void setup() {
    NoGround no_ground = NoGround();
    NoFloor no_floor = NoFloor();

    if (no_ground.pass(no_floor.yes())) {
        no_ground.pass(no_floor.no());
    }
};

void loop() {
    delay(millis());
};
