#ifndef _main_hpp_
#define _main_hpp_

#include <Arduino.h>

#include "ground/console.hpp"

/// it is possible to customize the settings for the serial console.
/// @see ConsoleParam for further details
ConsoleParam cparam = ConsoleParam(
    115200,             ///< customize baud rate
    SERIAL_8N1,         ///< choose serial configuration
    "\n\r",             ///< how to express a newline
    12, 14              ///< indent for headlines and following blocks
);


/// to start with default settings, it is possible to leave the parameters:
/// Console text = Console();

/// otherwise pass them as parameter:
Console text = Console(cparam);

void setup(void);
void loop(void);

#define PAUSE   512
#define BREAK   delay(PAUSE); text.text(cparam.newline);

#endif
