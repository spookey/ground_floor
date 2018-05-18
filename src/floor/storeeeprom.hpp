#ifndef _storeeeprom_hpp_
#define _storeeeprom_hpp_

#include <Arduino.h>
#include <EEPROM.h>

#include "ground/console.hpp"
#include "ground/shell.hpp"
#include "floor/store.hpp"

#ifdef ESP8266
    #define STORE_EE_BEGIN      EEPROM.begin(STORE_EE_SIZE)
    #define STORE_EE_COMMIT     EEPROM.commit()
    #define STORE_EE_SIZE       4096
        ///< defined by SPI_FLASH_SEC_SIZE in 'tools/sdk/include/spi_flash.h'
#else
    #define STORE_EE_BEGIN          ///< esp needs some setup, arduino doesn't
    #define STORE_EE_COMMIT         ///< as well as write commits
    #define STORE_EE_SIZE       EEPROM.length()
                                    ///< using either fixed or dynamic value
#endif


/// \class StoreEEPROM
/// \brief Store implementation to use on eepr0m
class StoreEEPROM : public Store {
public:
    /// constuctor @see StoreParam
    StoreEEPROM(Console& text, Shell& shell, StoreParam param)
    : Store(text, shell, param) {}
    /// constuctor with default values
    StoreEEPROM(Console& text, Shell& shell)
    : Store(text, shell) {}

    /// initialize the eepr0m
    /// - stops in infinite loop on offset error
    void setup(void) {
        STORE_EE_BEGIN;
        if (_param.ee_offset >= STORE_EE_SIZE) {
            String msg = _text.join(F("EEPROM offset too big ("), _text.join(
                String(_param.ee_offset), F(" >= "), String(STORE_EE_SIZE)
            ), F(")!!1!"));
            while (true) { _text.alarm(F("StoreEEPROM"), msg); }
        }
        Store::setup();
    }
private:
    /// helper to update one character in the eepr0m only if it differs
    /// - does the offset calculation
    bool ee_write(uint16_t pos, char val) {
        pos = _param.ee_offset + pos;
        if (STORE_EE_SIZE - pos <= 0) { return false; }
        char now; EEPROM.get(pos, now);
        if (now != val) {
            EEPROM.put(pos, val);
            STORE_EE_COMMIT;
        }
        return true;
    }

    /// helper to write one chunk into eepr0m
    uint16_t ee_write(uint16_t pos, String line) {
        uint16_t cur = 0;
        for (uint16_t idx = 0; idx < line.length(); idx++) {
            if (!ee_write(pos + idx, line.charAt(idx))) {
                _text.log(F("StoreEEPROM"), F("ee_write"));
                _text.llg(F("ERROR"), F("EEPROM is full"));
                return 0;
            }
            cur++;
        }
        ee_write(pos + cur + 1, '\0');
        return cur;
    }

    /// helper to read from eepr0m until val or real ``\0``
    /// - does the offset calculation
    String ee_read(uint16_t pos, char val = '\0') {
        pos = _param.ee_offset + pos;
        String result = String(); char now;
        while (STORE_EE_SIZE - pos > 0) {
            EEPROM.get(pos, now);
            if (now == val || now == '\0') { break; }
            result += now;
            pos++;
        }
        return result;
    }

    bool dump(void) {
        _text.log(F("StoreEEPROM"), F("dump"));
        _text.llg(F("offset"), String(_param.ee_offset));
        uint16_t cur = 0, tmp;
        for (uint8_t idx = 0; idx < _chn_idx; idx++) {
            tmp = ee_write(cur, pickle(_chain[idx]));
            if (tmp == 0) { return false; }
            cur += tmp;
        }
        _text.llg(F("SUCCESS"), F("dumped "), String(cur), F(" bytes"));
        return true;

    }

    bool load(bool soft = false) {
        _text.log(F("StoreEEPROM"), F("load"));
        _text.llg(F("offset"), String(_param.ee_offset));
        bool result = true;                 uint16_t pos = 0;
        Blob blob;                          String line;
        _text.llg();
        while (true) {
            line = ee_read(pos, _param.col_delimit);
            if (!line.length()) { break; }
            pos += line.length() + 1;
            blob = unpickle(line);
            if (soft) { _text.llg(blob.key, val(blob)); }
            else if (!append(blob)) { result = false; }
        }
        return result;
    }

    bool wipe(bool full = false) {
        _text.log(F("StoreEEPROM"), F("wipe"));
        uint16_t len;
        if (full) {
            _text.llg(F("full wipe"), F("format"));
            len = STORE_EE_SIZE;
        } else {
            _text.llg(F("quick wipe"), F("zeroing out"));
            len = ee_read(0).length();
        }
        for (uint16_t _ = 0; _ <= len; _++) { ee_write(len, '\0'); }
        _text.llg(F("wiped"), String(len), F(" bytes"));
        return true;
    }

};

#undef STORE_EE_BEGIN
#undef STORE_EE_COMMIT
#undef STORE_EE_SIZE

#endif
