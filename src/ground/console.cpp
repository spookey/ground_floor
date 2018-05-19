#include "ground/console.hpp"

void Console::setup(void) {
    Serial.begin(_param.baudrate, _param.serialconfig);
    while (!Serial) { delay(0.05); }
    text(join(
        fill(8, '\b'), fill(8, '\r'), _param.newline,
        fill(64, _param.initialize)
    ));
}
// TEXT PROCESSING //
String Console::fill(uint16_t width, char filler) {
    String result; while(width--) { result += filler; }
    return result;
}
String Console::clip(char left, char right, String txt, uint8_t repeat) {
    return join(fill(repeat, left), txt, fill(repeat, right));
}
String Console::box(char filler, String txt, uint8_t spacing) {
    String line = join(
        clip('+', fill((txt.length() + (2 * spacing)), '-')), _param.newline
    );
    return join(line, clip('|', clip(filler, txt, spacing)), _param.newline, line);
}
String Console::pad(char chomped, char filler, String txt, bool prepend, uint16_t width) {
    const uint16_t len = txt.length();
    if (width <= 2) { return fill(width, chomped); }
    if (width >= len) {
        String empty = fill((width - len), filler);
        return (prepend ? join(empty, txt) : join(txt, empty));
    } else {
        return join(txt.substring(0, (width - 2)), fill(2, chomped));
    }
}
// SERIAL OUTPUT //
void Console::raw(int data) { Serial.write(data); }
void Console::raw(char chr) { Serial.write(chr); }

void Console::text(String txt, bool newline) {
    Serial.print(join(txt, (newline ? _param.newline : ""))); Serial.flush();
}
// LOGGING //
void Console::log(String topic, String txt) {
    topic.toUpperCase();
    text(join(
        _param.newline, pad(topic, false, _param.block_part),
        String(_param.filler), txt
    ));
}
void Console::llg(String subtopic, String txt) {
    text(join(
        pad(subtopic, true, _param.block_full), F(":"),
        String(_param.filler), txt
    ));
}
void Console::llg(String txt) {
    txt.trim();
    llg(F("::"), (
        txt.length() ? join(txt, String(_param.filler)) : F("")
    ), F(":::"));
}
// SERIAL INPUT //
char Console::collect(char nothing) {
    const int32_t value = (Serial.available() ? Serial.read() : nothing);
    return ((value >= 0) ? ((char) value) : nothing);
}
bool Console::brake(unsigned long wait) {
    const unsigned long finish = wait + millis();
    while(millis() < finish) {
        if (collect() != _param.nocollect) { return true; }
        else { delay(0.05); }
    }
    return false;
}
// SERVICE //
void Console::alarm(String topic, String reason, unsigned long wait) {
    while (true) {
        log(topic, F("ALARM!"));
        llg(F("reason"), reason);
        llg(F("wait"), String(wait));
        if (brake(wait)) { return; }

        for (uint8_t sos = 0; sos < 3; sos++) {
            for (uint8_t _ = 0; _ < 3; _++) {
                for (uint16_t stp = 0; stp < 512 * ((sos == 1) ? 4 : 1); stp++) {
                    raw('\b');
                }
                if (brake(512)) { return; }
            }
            if (brake(512)) { return; }
        }
    }
}
String Console::get_uptime(void) {
    static char result[16];
    unsigned long sec = millis() / 1000;
    unsigned long min = sec / 60;   sec -= min * 60;
    unsigned long hrs = min / 60;   min -= hrs * 60;
    unsigned long day = hrs / 24;   hrs -= day * 24;
    if (day >= 1) { sprintf(result, "%dd %02d:%02d:%02d",
        (uint8_t) day,  (uint8_t) hrs, (uint8_t) min, (uint8_t) sec);
    } else {        sprintf(result, "%02d:%02d:%02d",
                        (uint8_t) hrs, (uint8_t) min, (uint8_t) sec);
    }
    return String(result);
}
