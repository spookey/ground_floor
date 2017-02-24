#ifndef _storespiffs_hpp_
#define _storespiffs_hpp_

#include <Arduino.h>
#ifdef ESP8266
    #include <FS.h>
#endif
#include "ground/console.hpp"
#include "ground/shell.hpp"
#include "floor/store.hpp"

/// \class StoreSPIFFS
/// \brief Store implementation to use on spiffs
/// \brief this only works for esp, not on arduino..
class StoreSPIFFS : public Store {
public:
    /// constuctor @see StoreParam
    StoreSPIFFS(Console& text, Shell& shell, StoreParam param)
    : Store(text, shell, param) {}
    /// constuctor with default values
    StoreSPIFFS(Console& text, Shell& shell)
    : Store(text, shell) {}

    /// mount the spiffs
    /// - stops in infinite loop on mount error
    void setup(void) {
#ifdef ESP8266
        if (!SPIFFS.begin()) {
            while (true) { _text.alarm(F("StoreSPIFFS"), F("could not mount SPIFFS")); }
        }
#endif
        Store::setup();
    }

private:
    bool dump(void) {
#ifndef ESP8266
        return false;
#else
        _text.log(F("StoreSPIFFS"), F("dump"));
        _text.llg(F("filename"), _param.filename);
        File file = SPIFFS.open(_param.filename, "w+");
        if (!file) {
            _text.llg(F("ERROR"), F("open failed"));
            return false;
        }
        if (!file.seek(0, SeekSet)) {
            _text.llg(F("ERROR"), F("rewind failed"));
            return false;
        }
        uint16_t size = 0;
        for (uint8_t idx = 0; idx < _chn_idx; idx++) {
            size += file.print(pickle(_chain[idx]));
        }
        _text.llg(F("SUCCESS"), F("dumped "), String(size), F(" bytes"));
        file.close();
        return true;
#endif
    }

    bool load(bool soft = false) {
#ifndef ESP8266
        return false;
#else
        _text.log(F("StoreSPIFFS"), F("load"));
        _text.llg(F("filename"), _param.filename);
        if (!SPIFFS.exists(_param.filename)) {
            _text.llg(F("ERROR"), F("file does not exist"));
            return false;
        }
        File file = SPIFFS.open(_param.filename, "r");
        if (!file) {
            _text.llg(F("ERROR"), F("open failed"));
            return false;
        }
        if (!file.seek(0, SeekSet)) {
            _text.llg(F("ERROR"), F("rewind failed"));
            return false;
        }
        Blob blob;
        bool result = true;
        _text.llg();
        while (file.available()) {
            blob = unpickle(file.readStringUntil(_param.col_delimit));
            if (soft) { _text.llg(blob.key, val(blob)); }
            else if (!append(blob)) { result = false; }
        }
        file.close();
        return result;
#endif
    }

    bool wipe(bool full = false) {
#ifndef ESP8266
        return false;
#else
        _text.log(F("StoreSPIFFS"), F("wipe"));
        if (full) {
            _text.llg(F("full wipe"), F("format"));
            SPIFFS.format();
        } else {
            _text.llg(F("quick wipe"), F("zeroing out"));
            _text.llg(F("filename"), _param.filename);
            File file = SPIFFS.open(_param.filename, "w+");
            if (!file || !file.seek(0, SeekSet)) {
                _text.llg(F("ERROR"), F("file access error"));
                return false;
            }
            file.print(String());
            file.close();
        }
        return true;
#endif
    }
};


#endif
