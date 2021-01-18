#include "main.hpp"

void demo() {
    // creating strings is easy:
    Str<16> hello = Str<16>("Hello");
    // or use the short forms:
    Str16 world = Str16("World");

    // to pass it around, use the pointer:
    Serial.println(hello.c_str());
    // there is also some shortcut:
    Serial.println(*world);

    Serial.println();

    // getting information:
    if (!hello.empty() || !world) {
        // this is how much space was assigned (=16):
        Serial.print("(Hello) size:   "); Serial.println(hello.size());
        Serial.print("(World) size:   "); Serial.println(world.size());

        // this is how much is in there (=5):
        Serial.print("(Hello) length: "); Serial.println(hello.length());
        Serial.print("(World) length: "); Serial.println(world.length());

        // this is how much space is left (=16 - 5 == 11):
        Serial.print("(Hello) left:   "); Serial.println(hello.left());
        Serial.print("(World) left:   "); Serial.println(world.left());
    }

    // comparisons:
    if (!hello.equals(world)) {
        // or use the operators:
        if (hello == hello) {
            if (world != world) {
                Serial.println("> Unreachable statement <");
            }
        }
    }
    // or compare beginnings:
    if (hello.starts_with("hell")) {
        // ... and endings:
        if (world.ends_with("olrd")) {
            if (hello.starts_with(world)) {
                Serial.println("> Unreachable statement <");
            }
        }
    }

    Serial.println();

    // clear and reassign:
    hello.set("Goodbye");
    world.clear();

    // hello is now longer (=7):
    Serial.print("(Hello) length: "); Serial.println(hello.length());
    // world is now empty (=0):
    Serial.print("(World) length: "); Serial.println(world.length());

    Serial.println();

    // append and prepend:
    hello.append(',');
    world.prepend("World");

    // hello is now longer (=8):
    Serial.print("(Hello) length: "); Serial.println(hello.length());
    // world is reset now (=5):
    Serial.print("(World) length: "); Serial.println(world.length());

    // returns false if remaining space is too short:
    if (hello.prepend("long text with more than 16 characters")) {
        // ...and the original content stays intact:
        if (world.append("again some lengthy text")) {
            Serial.println("> Unreachable statement <");
        }
    }
};
