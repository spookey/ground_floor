#ifndef _console_hpp_
#define _console_hpp_

#include <Arduino.h>

/// switch serial config type between arduino and esp
#ifdef ESP8266
    #define SERIALCONFIG_T      SerialConfig
#else
    #define SERIALCONFIG_T      uint8_t
#endif

/// \struct ConsoleParam
/// \brief stores parameters for the serial setup
/// \brief pass this into Console::Console
struct ConsoleParam {
    unsigned long baudrate; SERIALCONFIG_T serialconfig;
    String newline; uint16_t block_part; uint16_t block_full;
    char filler; char chomped; char nocollect; char initialize;

    ConsoleParam(
        unsigned long baudrate = 115200,            ///< baud rate
        SERIALCONFIG_T serialconfig = SERIAL_8N1,   ///< serial configuration
        String newline = String("\n\r"),            ///< line endings
        uint16_t block_part = 12,   ///< headline indent width
        uint16_t block_full = 14,   ///< indent for follow up blocks
        char filler = ' ',          ///< when drawing whitespace, use this char
        char chomped = '.',         ///< use this char if text got stripped away
        char nocollect = '\0',      ///< what to return if no character was collected
        char initialize = '^'       ///< clean up screen on start by repeating this char
    ) : baudrate(baudrate), serialconfig(serialconfig), newline(newline),
        block_part(block_part), block_full(block_full), filler(filler),
        chomped(chomped), nocollect(nocollect), initialize(initialize) {}
};

#undef SERIALCONFIG_T

/// \class Console
/// \brief handles communication over the serial wire
/// \brief helps with string processing and offers logging
class Console {
public:
    /// constuctor @see ConsoleParam
    Console(
        const ConsoleParam& param
    ) : _param(param) {};
    /// constuctor with default values
    Console(
        void
    ) : _param(ConsoleParam()) {};

    /// use this to setup and begin serial communication
    void setup(void);

// TEXT PROCESSING //
    /// combine two strings
    /// \return ``'aa' + 'bb'``
    String join(String aa, String bb);
    /// combine three strings
    String join(String aa, String bb, String cc);
    /// combine four strings
    String join(String aa, String bb, String cc, String dd);

    /// generate repeating strings
    /// \param filler character to repeat
    /// \param width how often to repat the character
    /// \return ``width * 'filler'``
    String fill(char filler, uint16_t width);
    /// alias for Console::fill with default filler
    String fill(uint16_t width) {
        return this->fill(width, this->_param.filler);
    }

    /// embrace a string
    /// \return ``(repeat * 'left') + 'txt' + (repeat * 'right')``
    String clip(
        char left, char right, String txt, uint8_t repeat = 1
    );
    /// alias for Console::clip with equal strings left and right
    String clip(char clamp, String txt, uint8_t repeat = 1) {
        return this->clip(clamp, clamp, txt, repeat);
    }

    /// draw a box around text
    /// \param txt guess what
    /// \param filler draw whitespace using this character
    /// \param spacing size of gap between ``|`` and ``txt``
    /// \return e.g.
    ///
    ///     +-------------+
    ///     | Wayne-Train |
    ///     +-------------+
    String box(char filler, String txt, uint8_t spacing = 1);
    /// alias for Console::box with default filler
    String box(String txt, uint8_t spacing = 1) {
        return this->box(this->_param.filler, txt, spacing);
    }

    /// align text in a grid
    /// \param txt content
    /// \param prepend left or right padding in result
    /// \param width total output width
    /// \param filler the whitespace character
    /// \param chomped use this character to signal ``txt.length() >= width``
    String pad(
        char chomped, char filler, String txt, bool prepend, uint16_t width
    );
    /// alias for Console::pad with default chomp character
    String pad(char filler, String txt, bool prepend, uint16_t width) {
        return this->pad(
            this->_param.chomped, filler,
            txt, prepend, width
        );
    }
    /// alias for Console::pad with default chomp and filler characters
    String pad(String txt, bool prepend, uint16_t width) {
        return this->pad(
            this->_param.chomped, this->_param.filler,
            txt, prepend, width
        );
    }

// SERIAL OUTPUT //
    /// write raw stuff to the serial
    /// \param data content
    void raw(int data);
    /// raw writer for single characters
    /// \param chr content
    void raw(char data);

    /// write complete strings to the serial
    /// \param txt content to write
    /// \param newline append line ending
    /// call it with empty parameters to just print a newline
    void text(String txt = "", bool newline = true);

// LOGGING //
    /// begin with a logging block
    /// \post followed by Console::llg
    /// \param topic where does the logging call come from?
    /// \param txt log message
    void log(String topic, String txt);
    /// log two message strings
    void log(String topic, String aa, String bb) {
        this->log(topic, this->join(aa, bb));
    }
    /// log three message strings
    void log(String topic, String aa, String bb, String cc) {
        this->log(topic, this->join(aa, bb, cc));
    }
    /// log four message strings
    void log(String topic, String aa, String bb, String cc, String dd) {
        this->log(topic, this->join(aa, bb, cc, dd));
    }

    /// continue a logging block, indented
    /// \pre follwing by Console::log
    /// \param subtopic whatever
    /// \param txt log message
    void llg(String subtopic, String txt);
    /// llg two message strings
    void llg(String topic, String aa, String bb) {
        this->llg(topic, this->join(aa, bb));
    }
    /// llg three message strings
    void llg(String topic, String aa, String bb, String cc) {
        this->llg(topic, this->join(aa, bb, cc));
    }
    /// llg four message strings
    void llg(String topic, String aa, String bb, String cc, String dd) {
        this->llg(topic, this->join(aa, bb, cc, dd));
    }
    /// short form for llg to use as empty divider
    void llg(String txt="");

// SERIAL INPUT //
    /// read one character from the serial
    /// \param nothing what to return if no input was present
    char collect(char nothing);
    /// Console::collect shortcut with defaults
    char collect(void) { return collect(this->_param.nocollect); }
    /// some kind of breakable delay - stops on keypress
    /// (timing is not exact!)
    ///
    /// ignores entries set in ConsoleParam::nocollect
    bool brake(unsigned long wait);

// SERVICE //
    /// send sos in morse code using the tx led of the serial device
    /// (loops until user intervention - press the any key :)
    /// \param topic who is calling for help?
    /// \param reason what happened?
    /// \param wait time bewteen alarm signals
    void alarm(String topic, String reason, unsigned long wait = 2048);

    /// outputs ``millis()`` output in human readable format
    /// \return ``23:42:05``
    /// \return ``5d 23:42:05``
    /// \return ``23d 23:42:05``
    String get_uptime(void);

    /// make defined newline characters available for others
    String get_newline(void) { return this->_param.newline; }
    /// tell me more about whitespace..
    char get_filler(void) { return this->_param.filler; }

private:
    const ConsoleParam& _param;      ///< stores current ConsoleParam

};

#endif
