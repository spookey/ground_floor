#ifndef _nothing_hpp_
#define _nothing_hpp_

#include <Arduino.h>

/**
 * This class does nothing (useful)
 */
class Nothing {
public:
    /** Constructor */
    Nothing() {}
    /** Destructor */
    ~Nothing() {}

private:
    /** yes or no */
    struct Reply {
        const bool yes = true;
        const bool no = false;
    };

protected:
    /** initialize reply */
    const Reply reply = Reply();

public:
    /** does nothing */
    void none() {};

    /**
     * Checks bool
     * @param some value
     * @return Reply.yes or Reply.no
     */
    bool is(const bool some) {
        if (some) {
            return reply.yes;
        }
        return reply.no;
    }
};

#endif
