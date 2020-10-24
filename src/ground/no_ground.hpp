#ifndef _no_ground_hpp_
#define _no_ground_hpp_

#include "Arduino.h"

struct NoGround {
    /** some constructor */
    NoGround() {}
    /** cleanup */
    ~NoGround() {}

private:
    /** does absolutely nothing */
    void none() {}

public:
    /** shit in, shit out
     * @param some data
     * @return the same
     * */
    bool pass(bool some) { return some; }

};

#endif
