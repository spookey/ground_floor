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
    Str() { set(_NIL); }
    /**
     * Construct string
     * @param str preset text
     */
    explicit Str(const char* str) { set(str); }
    /**
     * Construct string from character
     * @param chr preset character
     */
    Str(const char chr) { set(chr); }
    /** Desctructor */
    ~Str() {}


private:
    /** internal buffer */
    char _str[1 + LEN] = {_NIL};


public:
    /**
     * @param str new content string
     * @return if it worked
     */
    template<size_t SLEN>
    bool set(Str<SLEN> str) { return set(str.c_str()); }
    /**
     * @param str new content string
     * @return if it worked
     */
    bool set(const char* str) {
        const size_t len = snprintf(_str, sizeof(_str), "%s", str);
        return (0 <= len && sizeof(_str) > len);
    }
    /**
     * @param chr new content character
     * @return if it worked
     */
    bool set(const char chr) {
        const char buf[] = {chr, _NIL};
        return set(buf);
    }
    /**
     * @return if clearing worked
     */
    bool clear() { return set(_NIL); }


public:
    inline const char* operator *() { return _str; }
    /** @return pointer to character array */
    inline const char* c_str() { return _str; }


public:
    /** @return designated size of character array */
    inline size_t size() { return LEN; }
    /** @return length of content */
    inline size_t length() { return strnlen(_str, LEN); }
    /** @return how many space is left */
    inline size_t left() { return LEN - length(); }

    inline bool operator !() { return empty(); }
    /** @return true if length() == 0 */
    inline bool empty() { return (0 == length()); }


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
        const char buf[] = {chr, _NIL};
        return equals(buf);
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
        const char buf[] = {chr, _NIL};
        return starts_with(buf);
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
        const char buf[] = {chr, _NIL};
        return ends_with(buf);
    }


public:
    /**
     * @param str other string
     * @return if it worked
     */
    template<size_t SLEN>
    bool append(Str<SLEN> str) { return append(str.c_str()); }
    /**
     * @param str other string
     * @return if it worked
     */
    bool append(const char* str) {
        char buf[1 + LEN] = {_NIL};
        const size_t len = snprintf(buf, sizeof(buf), "%s%s", _str, str);
        if (0 <= len && sizeof(buf) > len) { return set(buf); }
        return false;
    }
    /**
     * @param chr some character
     * @return if it worked
     */
    bool append(const char chr) {
        const char buf[] = {chr, _NIL};
        return append(buf);
    }

    /**
     * @param str other string
     * @return if it worked
     */
    template<size_t SLEN>
    bool prepend(Str<SLEN> str) { return prepend(str.c_str()); }
    /**
     * @param str other string
     * @return if it worked
     */
    bool prepend(const char* str) {
        char buf[1 + LEN] = {_NIL};
        const size_t len = snprintf(buf, sizeof(buf), "%s%s", str, _str);
        if (0 <= len && sizeof(buf) > len) { return set(buf); }
        return false;
    }
    /**
     * @param chr some character
     * @return if it worked
     */
    bool prepend(const char chr) {
        const char buf[] = {chr, _NIL};
        return prepend(buf);
    }
};

typedef     Str<8>      Str8;       /**< tiny */
typedef     Str<16>     Str16;      /**< small */
typedef     Str<32>     Str32;      /**< medium */
typedef     Str<64>     Str64;      /**< big */
typedef     Str<128>    Str128;     /**< huge */

}
#endif
