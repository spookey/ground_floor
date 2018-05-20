#ifndef _main_hpp_
#define _main_hpp_

#include <Arduino.h>

/// include the header file - no other dependencies needed.
#include "ground/console.hpp"

void setup(void);
void loop(void);

#define PAUSE   512
#define BREAK   delay(PAUSE); text.text(); text.text();

#endif