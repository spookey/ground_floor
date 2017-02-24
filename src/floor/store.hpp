#ifndef _store_hpp_
#define _store_hpp_

#include <Arduino.h>
#include "ground/console.hpp"
#include "ground/shell.hpp"

/// \struct Store
/// \brief stores parameters for the store itself
/// \brief pass this into Store::Store
struct StoreParam{
    uint8_t chain_limit;
    uint16_t ee_offset;                     String filename;
    char col_delimit;                       char row_delimit;

    StoreParam(
#ifdef ESP8266
        uint8_t chain_limit = 16,           ///< no space left on device
#else
        uint8_t chain_limit = 8,            ///< how many items may be stored
#endif
        uint16_t ee_offset = 0,             ///< eepr0m offset
        String filename = "/config",        ///< spiffs filename
        char col_delimit = '\n',            ///< blob delimiter
        char row_delimit = '\t'             ///< element delimiter
    ) : chain_limit(chain_limit), ee_offset(ee_offset), filename(filename),
    col_delimit(col_delimit), row_delimit(row_delimit) {}
};

/// \enum StoreFlags
/// use these flags for special storage options
enum StoreFlags {
    STORE_FLAG_PLAIN = 0x00,
    STORE_FLAG_HIDDEN = 0x17,
    STORE_FLAG_VOID = 0xfe,
};

/// \class Store
/// \brief store stuff inside the device
/// \brief do not use this one directly, use one of the subclasses!
class Store {
public:
    /// constuctor @see StoreParam
    Store(Console& text, Shell& shell, StoreParam param)
    : _text(text), _shell(shell), _param(param) {
        _chain = new Blob[_param.chain_limit];
    }
    /// constuctor with default values
    Store(Console& text, Shell& shell)
    : Store(text, shell, StoreParam()) {}

    /// to register shell commands and load config
    void setup(void);

private:
    virtual bool dump(void) = 0;                ///<  write to storage media
    virtual bool load(bool soft = false) = 0;   ///< read from storage media
    virtual bool wipe(bool full = false) = 0;   ///< erase storage media

protected:
    /// \struct Blob
    /// \brief key value mapping with flags!
    struct Blob { String key; String value; uint8_t flag; };

// HANDLING //
    /// helper to output value according to flags
    String val(Blob blob);

private:
    /// retrieve raw store entries by key
    /// \return empty Store::Blob with flag STORE_FLAG_VOID if key not present
    Blob get(String key);
    /// updates flag and value of existing store entry
    bool change(Blob blob);

protected:
    /// update existing or add new store entry in chain
    bool append(Blob blob);

public:
    /// create a new store entry in the chain
    /// \param key where to find the data
    /// \param preset what to store as default value
    /// \param flag how to handle/display it
    /// \return ``true`` if value present, ``false`` otherwise (store full).
    bool ensure(String key, String preset, uint8_t flag = STORE_FLAG_PLAIN);

    /// quick lookup for values
    String get(String key, String fallback);

// CONVERT //
protected:
    /// take a Blob, pickle it
    String pickle(Blob blob);
    /// take a String, unpickle it
    Blob unpickle(String line);

// COMMANDS //
private:
    shell_t cmd_store(String arg);          ///< load/dump/drop/wipe store
    shell_t cmd_conf(String arg);           ///< edit config values

protected:
    Console& _text;                         ///< keep console for in/output here
    Shell& _shell;                          ///< keep shell for commands here
    const StoreParam _param;                ///< stores current StoreParam

    Blob* _chain;                           ///< (╯°□°）╯︵ ┻━┻
    uint8_t _chn_idx = 0;                   ///< current position in chain

};

#endif
