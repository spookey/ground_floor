#include "main.hpp"

/// it is possible to customize the settings for the serial console.
/// @see ConsoleParam for further details
ConsoleParam cparam = ConsoleParam(
    115200,         ///< customize baud rate
    SERIAL_8N1,     ///< choose serial configuration
    "\n\r",         ///< how to express a newline
    12, 14          ///< indent widths for headlines and following blocks
);


/// to start with default settings, it is possible to leave the parameters:
/// Console text = Console();

/// otherwise pass them as parameter:
Console text = Console(cparam);


void setup(void) {
    /// before doing anything else a call to .setup() is needed.
    /// this will initialize the serial communication, and prepare the screen
    text.setup();
}

void loop(void) {

// PRINTING //

    /// simple text printing is pretty simple...
    text.text("this is just text");

    BREAK

    /// it is possible to leave the newline to combine strings on screen:
    text.text("something ", false);
    text.text("is ", false);
    text.text("wrong ", false);
    text.text("!!!", true);

    BREAK

    /// if you have single characters use the .raw() function:
    text.raw('o'); text.raw('m'); text.raw('f'); text.raw('g');
    text.text("!!!"); ///< add a final .text() call to flush the screen...
    /// the same using codepoints:
    text.raw(111); text.raw(109); text.raw(102); text.raw(103);
    text.text("!!!");

    BREAK
    BREAK

// STRING PROCESSING //

    /// joing strings is pretty straight forward
    text.text(text.join("lol", "cat"));
    /// up to four parameters are available
    text.text(text.join("very ", "long ", "lol", "cat"));
    /// if you need more, nest the .join() like this:
    text.text(text.join(
        "the ",
        text.join("very ", "long ", "lol", "cat "),
        text.join("will ", "eat ", "you ", "alive "),
        "!!!"
    ));

    BREAK

    /// to generate repeating patterns use .fill()
    text.text(text.fill(8, '*'));

    /// or using the default fill character (default: space)
    /// @see ConsoleParam
    text.text(text.fill(8));

    BREAK

    /// to shift strings left/right use .clip()
    text.text(text.clip('l', 'r', "YOLO", 8));
    /// or with equal fillers:
    text.text(text.clip('-', "YOLO", 8));

    BREAK

    /// draw text inside boxes
    text.text(text.box("Wayne-Train"));

    BREAK

    /// align text in a grid
    /// text that overflows will be chomped (replaced by '*')
    /// space that is left over will be filled (with '-')
    text.text(text.pad('*', '-', "ROFLCOPTER", true, 6));
    /// or using the default chomp and fill characters
    /// @see ConsoleParam
    text.text(text.pad("ROFLCOPTER", false, 8));

    BREAK
    BREAK

// LOG MESSAGES //

    /// to introduce some proper way of logging look at this:
    /// think of a topic ("important") and some tag words first ("whatever")
    text.log("important", "what", "ever", "!");
    /// to continue use the .llg() function
    text.llg("state", "broken");
    text.llg("message", "kernel", "panic");
    text.llg("subdivider");
    text.llg("recover", "impossible");

    ///< using this pattern it is very easy to print key-value pairs
    ///< and categorize them..

    BREAK
    BREAK

// SERIAL INPUT //

    /// you can listen to single keystrokes
    char typed = text.collect('.');
    ///< if nothing was typed '.' will be returned:
    text.text(text.join("you typed: ", String(typed)));

    BREAK

    /// it is also possible to wait some time for a keystroke
    if (text.brake(512)) {
        text.text("a winner is you!");
    } else {
        text.text("you're doing it wrong!");
    }

    BREAK
    BREAK

// SERVICE //

    /// bonus feature - get the uptime in a human readable format
    text.log("uptime", text.get_uptime());

    BREAK

    /// something went very wrong? just break and send some sos-signal
    /// (look at the tx led)
    text.alarm("panic", "press the any key to continue");

    BREAK
    BREAK
}
