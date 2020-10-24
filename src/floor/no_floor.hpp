#ifndef _no_floor_hpp_
#define _no_floor_hpp_

#include "Arduino.h"

class NoFloor {
public:
    /** constructor */
    NoFloor() {}
    /** cleanup */
    ~NoFloor() {}

    /** say yes
     * @return true
     * */
    bool yes() { return true; }
    /** say no!
     * @return false
     * */
    bool no() { return false; }
};

#endif
