#include "floor/store.hpp"

void Store::setup(void) {
    _shell.enroll(this, &Store::cmd_conf, F("conf"), F("config management utility"));
    _shell.enroll(this, &Store::cmd_store, F("store"), F("storage management utility"));
    if (!load()) { _text.alarm(F("Store"), F("load config failed!")); }
}
// HANDLING //
String Store::val(Store::Blob blob) {
    String res = String();
    switch (blob.flag) {
        case STORE_FLAG_PLAIN:  res = blob.value;           break;
        case STORE_FLAG_HIDDEN: res = _text.join(
                _text.fill(blob.value.length(), '*'),
                F(" ("), String(blob.value.length()), F(")")
            );                                              break;
        case STORE_FLAG_VOID:   res = F("__void__value__"); break;
        default:                                            break;
    }
    return _text.join(res, F(" ["), String(blob.flag, HEX), F("]"));
}
Store::Blob Store::get(String key) {
    for (uint8_t idx = 0; idx < _chn_idx; idx++) {
        if (_chain[idx].key == key) { return _chain[idx]; }
    }
    return Blob({key, String(), STORE_FLAG_VOID});
}
String Store::get(String key, String fallback) {
    Blob result = get(key);
    if (result.flag == STORE_FLAG_VOID) { return fallback; }
    return result.value;
}
bool Store::change(Store::Blob blob) {
    if (blob.flag == STORE_FLAG_VOID) { return false; }
    for (uint8_t idx = 0; idx < _chn_idx; idx++) {
        if (_chain[idx].key == blob.key) {
            _text.log(F("Store"), F("change"));
            _text.llg(F("key"), blob.key);
            _text.llg(F("from old"), val(_chain[idx]));
            _text.llg(F("to new"), val(blob));
            _chain[idx].value = blob.value;
            _chain[idx].flag = blob.flag;
            return true;
        }
    };
    return false;
}
bool Store::append(Store::Blob blob) {
    if (blob.flag == STORE_FLAG_VOID) { return false; }
    if (change(blob)) { return true; }
    _text.log(F("Store"), F("append"));
    _text.llg(F("key"), blob.key);
    if (_chn_idx < _param.chain_limit) {
        _text.llg(F("value"), val(blob));
        _chain[_chn_idx] = blob;
        _chn_idx++;
        return true;
    }
    _text.llg(F("ERROR"), F("chain is full"));
    return false;
}
bool Store::ensure(String key, String preset, uint8_t flag) {
    Blob blob = get(key);
    if (blob.flag != STORE_FLAG_VOID) { return true; }
    blob.value = preset;
    blob.flag = flag;
    return append(blob);
}
// CONVERT //
String Store::pickle(Store::Blob blob) {
    return _text.join(
        blob.key + _param.row_delimit,
        blob.value + _param.row_delimit,
        String(blob.flag, DEC) + _param.col_delimit
    );
}
Store::Blob Store::unpickle(String line) {
    String key = String();
    String value = String();
    uint8_t flag = STORE_FLAG_VOID;

    int32_t fst = line.indexOf(_param.row_delimit);
    int32_t snd = line.lastIndexOf(_param.row_delimit);
    if (fst != snd && fst > 0 && snd > 0) {
        key = line.substring(0, fst);
        value = line.substring(fst + 1, snd);
        flag = line.substring(snd + 1).toInt();
    }
    return Blob({key, value, flag});
}
// COMMANDS //
shell_t Store::cmd_store(String arg) {
    _text.log(F("Store"), F("store utility"));
    arg.toLowerCase();
    if (!arg.length()) {
        _text.llg(F("store load"), F("load config from store"));
        _text.llg(F("store show"), F("show config inside store"));
        _text.llg(F("store dump"), F("write config into store"));
        _text.llg(F("store wipe"), F("erase config inside store"));
        _text.llg(F("store wipeall"), F("erase ALL THE store"));
        return SHELL_ARGUMENT_MISSING;
    } else if (arg == "wipeall") {
        return (wipe(true) ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "wipe") {
        return (wipe(false) ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "dump") {
        return (dump() ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "show") {
        return (load(true) ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "load") {
        return (load(false) ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    }
    return SHELL_ARGUMENT_ERROR;
}
shell_t Store::cmd_conf(String arg) {
    int32_t split = arg.indexOf(" ");
    _text.log(F("Store"), F("config utility"));
    if (arg.length()) {
        String key = ((split > 0) ? arg.substring(0, split) : arg);
        key.trim();
        _text.llg(F("key"), key);
        Blob blob = get(key);
        if (blob.flag == STORE_FLAG_VOID) {
            _text.llg(F("ERROR"), F("no such key"));
            return SHELL_STORE_ACCESS_ERROR;
        }
        if (split > 0) {
            String value = arg.substring(split + 1);
            value.trim();
            blob.value = value;
            if (!change(blob)) { return SHELL_STORE_ACCESS_ERROR; }
        } else {
            _text.llg(F("value"), val(blob));
        }
        return SHELL_OK;
    }
    _text.llg(F("items"), String(_chn_idx));
    _text.llg(F("free"), String(_param.chain_limit - _chn_idx));
    if (!_chn_idx) {
        _text.llg(F("empty"));
        return SHELL_OK;
    }
    _text.llg();
    for (uint8_t idx = 0; idx < _chn_idx; idx++) {
        _text.llg(_chain[idx].key, val(_chain[idx]));
    }
    return SHELL_OK;
}
