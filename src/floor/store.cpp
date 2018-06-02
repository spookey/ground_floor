#include "floor/store.hpp"

void Store::setup(void) {
    this->_shell.enroll(
        this, &Store::cmd_conf,
        F("conf"), F("config management utility")
    );
    this->_shell.enroll(
        this, &Store::cmd_store,
        F("store"), F("storage management utility")
    );
    if (!this->load()) {
        this->_text.alarm(F("Store"), F("load config failed!"));
    }
}
// HANDLING //
String Store::val(Store::Blob blob) {
    String res;
    switch (blob.flag) {
        case STORE_FLAG_PLAIN:  res = blob.value;           break;
        case STORE_FLAG_HIDDEN: res = this->_text.join(
                this->_text.fill('*', blob.value.length()),
                F(" ("), String(blob.value.length()), F(")")
            );                                              break;
        case STORE_FLAG_VOID:   res = F("__void__value__"); break;
        default:                res = "";                   break;
    }
    return this->_text.join(res, F(" ["), String(blob.flag, HEX), F("]"));
}
Store::Blob Store::get(String key) {
    for (uint8_t idx = 0; idx < this->_chn_idx; idx++) {
        if (this->_chain[idx].key == key) { return this->_chain[idx]; }
    }
    return Store::Blob({key, String(), STORE_FLAG_VOID});
}
String Store::get(String key, String fallback) {
    Store::Blob result = this->get(key);
    if (result.flag == STORE_FLAG_VOID) { return fallback; }
    return result.value;
}
bool Store::change(Store::Blob blob) {
    if (blob.flag == STORE_FLAG_VOID) { return false; }
    for (uint8_t idx = 0; idx < this->_chn_idx; idx++) {
        if (this->_chain[idx].key == blob.key) {
            this->_text.log(F("Store"), F("change"));
            this->_text.llg(F("key"), blob.key);
            this->_text.llg(F("from old"), val(this->_chain[idx]));
            this->_text.llg(F("to new"), val(blob));
            this->_chain[idx].value = blob.value;
            this->_chain[idx].flag = blob.flag;
            return true;
        }
    };
    return false;
}
bool Store::append(Store::Blob blob) {
    if (blob.flag == STORE_FLAG_VOID) { return false; }
    if (this->change(blob)) { return true; }
    this->_text.log(F("Store"), F("append"));
    this->_text.llg(F("key"), blob.key);
    if (this->_chn_idx < this->_param.chain_limit) {
        this->_text.llg(F("value"), val(blob));
        this->_chain[this->_chn_idx] = blob;
        this->_chn_idx++;
        return true;
    }
    this->_text.llg(F("ERROR"), F("chain is full"));
    return false;
}
bool Store::ensure(String key, String preset, uint8_t flag) {
    Store::Blob blob = this->get(key);
    if (blob.flag != STORE_FLAG_VOID) { return true; }
    blob.value = preset;
    blob.flag = flag;
    return this->append(blob);
}
// CONVERT //
String Store::pickle(Store::Blob blob) {
    return this->_text.join(
        blob.key + this->_param.row_delimit,
        blob.value + this->_param.row_delimit,
        String(blob.flag, DEC) + this->_param.col_delimit
    );
}
Store::Blob Store::unpickle(String line) {
    String key = String();
    String value = String();
    uint8_t flag = STORE_FLAG_VOID;

    int32_t fst = line.indexOf(this->_param.row_delimit);
    int32_t snd = line.lastIndexOf(this->_param.row_delimit);
    if (fst != snd && fst > 0 && snd > 0) {
        key = line.substring(0, fst);
        value = line.substring(fst + 1, snd);
        flag = line.substring(snd + 1).toInt();
    }
    return Store::Blob({key, value, flag});
}
// COMMANDS //
shell_t Store::cmd_store(String arg) {
    this->_text.log(F("Store"), F("store utility"));
    arg.toLowerCase();
    if (!arg.length()) {
        this->_text.llg(F("store load"), F("load config from store"));
        this->_text.llg(F("store show"), F("show config inside store"));
        this->_text.llg(F("store dump"), F("write config into store"));
        this->_text.llg(F("store wipe"), F("erase config inside store"));
        this->_text.llg(F("store wipeall"), F("erase ALL THE store"));
        return SHELL_ARGUMENT_MISSING;
    } else if (arg == "wipeall") {
        return (this->wipe(true)    ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "wipe") {
        return (this->wipe(false)   ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "dump") {
        return (this->dump()        ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "show") {
        return (this->load(true)    ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    } else if (arg == "load") {
        return (this->load(false)   ? SHELL_OK : SHELL_STORE_ACCESS_ERROR);
    }
    return SHELL_ARGUMENT_ERROR;
}
shell_t Store::cmd_conf(String arg) {
    int32_t split = arg.indexOf(" ");
    this->_text.log(F("Store"), F("config utility"));
    if (arg.length()) {
        String key = ((split > 0) ? arg.substring(0, split) : arg);
        key.trim();
        this->_text.llg(F("key"), key);
        Store::Blob blob = this->get(key);
        if (blob.flag == STORE_FLAG_VOID) {
            this->_text.llg(F("ERROR"), F("no such key"));
            return SHELL_STORE_ACCESS_ERROR;
        }
        if (split > 0) {
            String value = arg.substring(split + 1);
            value.trim();
            blob.value = value;
            if (!this->change(blob)) { return SHELL_STORE_ACCESS_ERROR; }
        } else {
            this->_text.llg(F("value"), this->val(blob));
        }
        return SHELL_OK;
    }
    this->_text.llg(F("items"), String(this->_chn_idx));
    this->_text.llg(
        F("free"), String(this->_param.chain_limit - this->_chn_idx)
    );
    if (!this->_chn_idx) {
        this->_text.llg(F("empty"));
        return SHELL_OK;
    }
    this->_text.llg();
    for (uint8_t idx = 0; idx < this->_chn_idx; idx++) {
        this->_text.llg(this->_chain[idx].key, this->val(this->_chain[idx]));
    }
    return SHELL_OK;
}
