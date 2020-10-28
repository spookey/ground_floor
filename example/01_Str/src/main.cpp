#include "main.hpp"

void demo() {
    // creating strings is easy
    Str<16> text = Str<16>("Hello");

    // to pass it around, use the pointer
    Serial.println(text.c_str());
    // there is also some shortcut:
    Serial.println(text.c());

    // getting information:
    if (!text.empty()) {
        // this is how much space was assigned (=16)
        Serial.print("size:   "); Serial.println(text.size());

        // this is how much is in there (=5)
        Serial.print("length: "); Serial.println(text.length());

        // this is how much space is left (=16 - 5 == 11)
        Serial.print("left:   "); Serial.println(text.left());
    }

    // comparing:
    if (text.equals(text)) {
        // or use the operators
        if (text == text) {
            if (text != text) {
                Serial.println("> Unreachable statement <");
            }
        }
    }
};
