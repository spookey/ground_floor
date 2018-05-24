#include "ground/console.hpp"

void Console::setup(void) {
    Serial.begin(this->_param.baudrate, this->_param.serialconfig);
    while (!Serial) { delay(0.05); }
    this->text(this->join(
        this->fill('\b', 8), this->fill('\r', 8),
        this->_param.newline,
        this->fill(this->_param.initialize, 64)
    ));
}
// TEXT PROCESSING //
String Console::join(String aa, String bb) {
    aa.concat(bb); return aa;
}
String Console::join(String aa, String bb, String cc) {
    aa.concat(bb); aa.concat(cc); return aa;
}
String Console::join(String aa, String bb, String cc, String dd) {
    aa.concat(bb); aa.concat(cc); aa.concat(dd); return aa;
}
String Console::fill(char filler, uint16_t width) {
    String result; while(width--) { result.concat(filler); }
    return result;
}
String Console::clip(char left, char right, String txt, uint8_t repeat) {
    return this->join(
        this->fill(left, repeat), txt, this->fill(right, repeat)
    );
}
String Console::box(char filler, String txt, uint8_t spacing) {
    String line = this->clip('+', this->fill('-', txt.length() + (2*spacing)));
    String cont = this->clip('|', this->clip(filler, txt, spacing));
    return this->join(
        this->join(line, this->_param.newline),
        this->join(cont, this->_param.newline),
        this->join(line, this->_param.newline)
    );
}
String Console::pad(
    char chomped, char filler, String txt, bool prepend, uint16_t width
) {
    const uint16_t len = txt.length();
    if (width <= 2) { return this->fill(chomped, width); }
    if (width >= len) {
        String empty = this->fill(filler, (width - len));
        return (prepend ? this->join(empty, txt) : this->join(txt, empty));
    }
    return this->join(txt.substring(0, (width - 2)), this->fill(chomped, 2));
}
// SERIAL OUTPUT //
void Console::raw(int data) { Serial.write(data); }
void Console::raw(char data) { Serial.write(data); }

void Console::text(String txt, bool newline) {
    if (newline) { txt = this->join(txt, this->_param.newline); }
    Serial.print(txt); Serial.flush();
}
// LOGGING //
void Console::log(String topic, String txt) {
    topic.toUpperCase();
    this->text(this->join(
        this->_param.newline,
        this->pad(topic, false, this->_param.block_part),
        String(this->_param.filler), txt
    ));
}
void Console::llg(String subtopic, String txt) {
    this->text(this->join(
        this->pad(subtopic, true, this->_param.block_full),
        F(":"), String(this->_param.filler), txt
    ));
}
void Console::llg(String txt) {
    txt.trim();
    if (txt.length()) { txt = this->join(txt, String(this->_param.filler)); }
    this->llg(F("::"), txt, F(":::"));
}
// SERIAL INPUT //
char Console::collect(char nothing) {
    const int32_t value = (Serial.available() ? Serial.read() : nothing);
    return ((value >= 0) ? ((char) value) : nothing);
}
bool Console::brake(unsigned long wait) {
    const unsigned long finish = wait + millis();
    while(millis() < finish) {
        if (collect(this->_param.nocollect) != this->_param.nocollect) {
            return true;
        }
        delay(0.05);
    }
    return false;
}
// SERVICE //
void Console::alarm(String topic, String reason, unsigned long wait) {
    while (true) {
        this->log(topic, F("ALARM!"));
        this->llg(F("reason"), reason);
        this->llg(F("wait"), String(wait));
        if (this->brake(wait)) { return; }

        for (uint8_t sos = 0; sos < 3; sos++) {
            for (uint8_t __ = 0; __ < 3; __++) {
                for (uint16_t _ = 0; _ < 512 * ((sos == 1) ? 4 : 1); _++) {
                    this->raw('\b');
                }
                if (this->brake(512)) { return; }
            }
            if (this->brake(512)) { return; }
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
