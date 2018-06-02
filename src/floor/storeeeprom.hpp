#ifndef _storeeeprom_hpp_
#define _storeeeprom_hpp_

#include <Arduino.h>
#include <EEPROM.h>

#include "ground/console.hpp"
#include "ground/shell.hpp"
#include "floor/store.hpp"

#ifdef ESP8266
    /// eepr0m setup for esp
    #define STORE_EE_BEGIN      EEPROM.begin(STORE_EE_SIZE)
    /// eepr0m commit for esp
    #define STORE_EE_COMMIT     EEPROM.commit()
    /// defined in 'tools/sdk/include/spi_flash.h' as SPI_FLASH_SEC_SIZE
    #define STORE_EE_SIZE       4096

#else
    /// empty - esp needs some setup, but arduino doesn't
    #define STORE_EE_BEGIN
    /// empty - arduino does not need to write commits
    #define STORE_EE_COMMIT
    /// to determine total size
    #define STORE_EE_SIZE       EEPROM.length()

#endif


/// \class StoreEEPROM
/// \brief Store implementation to use on eepr0m
class StoreEEPROM : public Store {
public:
    /// constuctor @see StoreParam
    StoreEEPROM(
        Console& text, Shell& shell, const StoreParam& param
    ) : Store(text, shell, param) {}
    /// constuctor with default values
    StoreEEPROM(
        Console& text, Shell& shell
    ) : Store(text, shell) {}

    /// initialize the eepr0m
    /// - stops in infinite loop on offset error
    void setup(void) {
        STORE_EE_BEGIN;
        if (this->_param.ee_offset >= STORE_EE_SIZE) {
            String msg = this->_text.join(
                F("EEPROM offset too big ("), this->_text.join(
                    String(this->_param.ee_offset),
                    F(" >= "), String(STORE_EE_SIZE)
                ), F(")!!1!")
            );
            while (true) { this->_text.alarm(F("StoreEEPROM"), msg); }
        }
        Store::setup();
    }
private:
    /// helper to update one character in the eepr0m only if it differs
    /// - does the offset calculation
    bool ee_write(uint16_t pos, char val) {
        pos = this->_param.ee_offset + pos;
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
            if (!this->ee_write(pos + idx, line.charAt(idx))) {
                this->_text.log(F("StoreEEPROM"), F("ee_write"));
                this->_text.llg(F("ERROR"), F("EEPROM is full"));
                return 0;
            }
            cur++;
        }
        this->ee_write(1 + pos + cur, '\0');
        return cur;
    }

    /// helper to read from eepr0m until val or real ``\0``
    /// - does the offset calculation
    String ee_read(uint16_t pos, char val = '\0') {
        pos = this->_param.ee_offset + pos;
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
        this->_text.log(F("StoreEEPROM"), F("dump"));
        this->_text.llg(F("offset"), String(_param.ee_offset));
        uint16_t cur = 0, tmp;
        for (uint8_t idx = 0; idx < this->_chn_idx; idx++) {
            tmp = this->ee_write(cur, this->pickle(this->_chain[idx]));
            if (tmp == 0) { return false; }
            cur += tmp;
        }
        this->_text.llg(F("SUCCESS"), F("dumped "), String(cur), F(" bytes"));
        return true;

    }

    bool load(bool soft = false) {
        this->_text.log(F("StoreEEPROM"), F("load"));
        this->_text.llg(F("offset"), String(this->_param.ee_offset));
        bool result = true;                 uint16_t pos = 0;
        Store::Blob blob;                   String line;
        this->_text.llg();
        while (true) {
            line = this->ee_read(pos, this->_param.col_delimit);
            if (!line.length()) { break; }
            pos += 1 + line.length();
            blob = this->unpickle(line);
            if (soft) { this->_text.llg(blob.key, this->val(blob)); }
            else if (!this->append(blob)) { result = false; }
        }
        return result;
    }

    bool wipe(bool full = false) {
        this->_text.log(F("StoreEEPROM"), F("wipe"));
        uint16_t len;
        if (full) {
            this->_text.llg(F("full wipe"), F("format"));
            len = STORE_EE_SIZE;
        } else {
            this->_text.llg(F("quick wipe"), F("zeroing out"));
            len = this->ee_read(0).length();
        }
        for (uint16_t _ = 0; _ <= len; _++) { this->ee_write(len, '\0'); }
        this->_text.llg(F("wiped"), String(len), F(" bytes"));
        return true;
    }

};

#undef STORE_EE_BEGIN
#undef STORE_EE_COMMIT
#undef STORE_EE_SIZE

#endif
