#ifndef _ground_str_hpp_
#define _ground_str_hpp_

#include "ground/_init/_init.hpp"

namespace gf {

/**
 * String implementation that keeps the data on the stack
 * to avoid memory fragmentation.
 */
template<size_t LEN>
class Str {
public:
    /** Construct an empty string */
    Str() { _str[LEN] = {_NIL}; }
    /**
     * Construct string
     * @param str preset text
     */
    explicit Str(const char* str) : Str() {
        snprintf(_str, sizeof(_str), str);
    }
    /**
     * Construct string from character
     * @param chr preset character
     */
    Str(const char chr) : Str() {
        _str[0] = chr; _str[1] = _NIL;
    }
    /** Desctructor */
    ~Str() {}


private:
    char _str[1 + LEN] = {_NIL};


public:
    inline const char* operator *() { return _str; }
    /** @return pointer to character array */
    inline const char* c_str() { return _str; }


public:
    /** @return designated size of character array */
    inline size_t size() { return LEN; }
    /** @return length of content */
    inline size_t length() { return strnlen(_str, sizeof(_str)); }
    /** @return how many space is left */
    inline size_t left() { return LEN - length(); }

    inline bool operator !() { return empty(); }
    /** @return true if length() == 0 */
    inline bool empty() { return _NIL == _str[0]; }


public:
    template<size_t SLEN>
    inline bool operator ==(Str<SLEN> str) { return equals(str.c_str()); }
    template<size_t SLEN>
    inline bool operator !=(Str<SLEN> str) { return !equals(str.c_str()); }
    /**
     * @param str other string
     * @return if same content
     */
    template<size_t SLEN>
    bool equals(Str<SLEN> str) { return equals(str.c_str()); }

    inline bool operator ==(const char* str) { return equals(str); }
    inline bool operator !=(const char* str) { return !equals(str); }
    /**
     * @param str other string
     * @return if same content
     */
    bool equals(const char* str) {
        return (0 == strncmp(_str, str, strnlen(str, sizeof(str))));
    }

    inline bool operator ==(const char chr) { return equals(chr); }
    inline bool operator !=(const char chr) { return !equals(chr); }
    /**
     * @param chr some character
     * @return if same content
     */
    bool equals(const char chr) {
        return (chr == _str[0]);
    }


public:
    /**
     * @param  str other string
     * @return if it starts with other string
     */
    template<size_t SLEN>
    bool starts_with(Str<SLEN> str) { return starts_with(str.c_str()); }
    /**
     * @param  str other string
     * @return if it starts with other string
     */
    bool starts_with(const char* str) {
        const size_t len = strlen(str);
        if (1 > len) { return true; }
        if (length() < len) { return false; }
        return (0 == strncmp(_str, str, len));
    }
    /**
     * @param  chr some character
     * @return if it starts with some character
     */
    bool starts_with(const char chr) {
        return (chr == _str[0]);
    }

    /**
     * @param  str other string
     * @return if it ends with other string
     */
    template<size_t SLEN>
    bool ends_with(Str<SLEN> str) { return ends_with(str.c_str()); }
    /**
     * @param  str other string
     * @return if it ends with other string
     */
    bool ends_with(const char* str) {
        const size_t len = strlen(str);
        if (1 > len) { return true; }
        if (length() < len) { return false; }
        return (0 == strncmp(_str + length() - len, str, len));
    }
    /**
     * @param  chr some character
     * @return if it ends with some character
     */
    bool ends_with(const char chr) {
        const size_t len = length();
        if (1 > len) { return false; }
        return (chr == _str[len - 1]);
    }


public:
    /**
     * @param str new content string
     * @return that string, but different content
     */
    template<size_t SLEN>
    Str set(Str<SLEN> str) { return set(str.c_str()); }
    /**
     * @param str new content string
     * @return that string, but different content
     */
    Str set(const char* str) {
        snprintf(_str, sizeof(_str), str);
        return *this;
    }
    /**
     * @param chr new content character
     * @return that string, but different content
     */
    Str set(const char chr) {
        _str[0] = chr, _str[1] = _NIL;
        return *this;
    }
    /**
     * @return that string, but empty
     */
    Str clear() { return set(_NIL); }

};

typedef     Str<8>      Str8;       /**< tiny */
typedef     Str<16>     Str16;      /**< small */
typedef     Str<32>     Str32;      /**< medium */
typedef     Str<64>     Str64;      /**< big */
typedef     Str<128>    Str128;     /**< huge */

}
#endif
