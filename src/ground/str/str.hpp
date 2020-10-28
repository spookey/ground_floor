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
    Str() {
        _str[LEN] = {_NIL};
    }
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
    /** @return pointer to character array */
    inline const char* c_str() { return _str; }
    /** @return c_str() */
    inline const char* c() { return c_str(); }

public:
    /** @return designated size of character array */
    size_t size() { return LEN; }
    /** @return length of content */
    size_t length() { return strnlen(_str, sizeof(_str)); }
    /** @return how many space is left */
    size_t left() { return LEN - length(); }
    /** @return true if length() == 0 */
    bool empty() { return _NIL == _str[0]; }

public:
    template<size_t OLEN>
    inline bool operator ==(Str<OLEN> str) { return equals(str.c_str()); }
    template<size_t OLEN>
    inline bool operator !=(Str<OLEN> str) { return !equals(str.c_str()); }
    /**
     * @param str other string
     * @return if same content
     */
    template<size_t OLEN>
    bool equals(Str<OLEN> str) { return equals(str.c_str()); }

    inline bool operator ==(Str str) { return equals(str.c_str()); }
    inline bool operator !=(Str str) { return !equals(str.c_str()); }
    /**
     * @param str other string
     * @return if same content
     */
    bool equals(Str str) { return equals(str.c_str()); }

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

};

}
#endif
