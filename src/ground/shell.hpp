#ifndef _shell_hpp_
#define _shell_hpp_

#include <Arduino.h>

#include "ground/console.hpp"

#define SHELL_KEY_BACKSP    8   ///< key code for backspace
#define SHELL_KEY_ESCAPE    27  ///< not present on some macbooks ;)
#define SHELL_KEY_RETURN    13  ///< code for return key

#define _SHELL_RETURNCODE(FUNC) \
    FUNC(0x00, SHELL_OK) \
    FUNC(0x12, SHELL_STORE_ACCESS_ERROR) \
    FUNC(0x22, SHELL_NETWORKING_ERROR) \
    FUNC(0xab, SHELL_ARGUMENT_MISSING) \
    FUNC(0xac, SHELL_ARGUMENT_ERROR) \
    FUNC(0xfe, SHELL_FATAL_ERROR) \
                                ///< return code to text mapping

#define _SHELL_ENLISTMENT(NUM, TXT) \
    TXT = NUM,
                                ///< use return codes in enum

/// \enum ShellCodes
/// use these return codes for enrolled callbacks
enum ShellCodes { _SHELL_RETURNCODE(_SHELL_ENLISTMENT) };

#undef _SHELL_ENLISTMENT

typedef uint8_t shell_t;        ///< use this as returncode

/// \struct ShellParam
/// \brief stores parameters for the shell
/// \brief pass this into Shell::Shell
struct ShellParam {
    String prompt;                      uint8_t command_limit;
    uint16_t buffer_limit;              bool double_echo;

    ShellParam(
        String prompt = "%>",           ///< intro prompt
#ifdef ESP8266
        uint8_t command_limit = 16,     ///< enrolled commands limit
        uint16_t buffer_limit = 64,     ///< limit of input buffer
#else
        uint8_t command_limit = 8,      ///< enrolled commands limit
        uint16_t buffer_limit = 32,     ///< limit of input buffer
#endif
        bool double_echo = true         ///< repeat input twice
    ) : prompt(prompt), command_limit(command_limit),
    buffer_limit(buffer_limit), double_echo(double_echo) {}
};


/// \class Shell
/// \brief launch commands over the serial interface
/// \brief enroll functions here, we'll call you back
class Shell {
public:
    /// constuctor @see ShellParam
    Shell(Console& text, ShellParam param) : _text(text), _param(param) {
        _commands = new Command[_param.command_limit];
        _buffer = new char[_param.buffer_limit];
    }
    /// constuctor with default values
    Shell(Console& text) : Shell(text, ShellParam()) {}

    /// capture input and react
    void loop(void);

private:
// COMMANDS //
    /// \struct Command
    /// \brief stores references to callback functions
    struct Command {
        void* owner_p;          ///< keep track of parent class
        uintptr_t member_p[4];  ///< raw function pointer of member
        shell_t (*callback_p)(void*, uintptr_t*, String);
                                ///< registered invoke function
        String name;            ///< command name
        String help;            ///< command help text

        /// locate addresses of enrolled method to call it
        /// \tparam T owner class type
        /// \param owner parent class pointer
        /// \param member callback function pointer
        /// \param args pass data to callback
        template<typename T>
        static shell_t callback(void* owner, uintptr_t* member, String args) {
            T* obj = static_cast<T*>(owner);
            shell_t (T::**mbr)(String) = reinterpret_cast<shell_t (T::**)(String)>(member);
            return (obj->**mbr)(args);
        }

        /// public interface to call back
        /// \param args content to pass into callback
        shell_t operator()(String args) { return callback_p(owner_p, member_p, args); }
    };

    /// implementation of command enrollment
    bool enroll(Command& cmd);

public:
    /// enroll some new command
    /// \tparam T owner class type
    /// \param owner enroller class
    /// \cond BREATHE_FAILS_HERE
    ///   \param member enrolled callback
    /// \endcond
    /// \param name command name
    /// \param help command help text
    template<typename T>
    bool enroll(T* owner,                   /// \cond BREATHE_FAILS_HERE
            shell_t (T::*member)(String),   /// \endcond
            String name, String help = "-") {
        Command* cmd = new Command;
        cmd->owner_p = static_cast<void*>(owner);
        *reinterpret_cast<shell_t (T::**)(String)>(cmd->member_p) = member;
        cmd->callback_p = &Command::callback<T>;
        cmd->name = name; cmd->help = help;
        return enroll(*cmd);
    }

private:
// HANDLING //
#ifdef ESP8266
    #define _SHELL_SWITCHCASE(NUM, TXT) \
        case NUM: text = #TXT; break;       ///< F() macro makes trouble on esp
#else
    #define _SHELL_SWITCHCASE(NUM, TXT) \
        case NUM: text = F(#TXT); break;    ///< return codes as switch
#endif
    /// \param code numeric return code
    /// \return text of return code
    String cmd_returncode(shell_t code) {
        String text;
        switch(code) { _SHELL_RETURNCODE(_SHELL_SWITCHCASE) }
        text.toLowerCase();
        text.replace("_", " ");
        return text.substring(6);
    }
#undef _SHELL_SWITCHCASE

    /// prints list of available commands
    void cmd_help(void);
    /// search enrolled and run Shell::cmd_check if found
    /// \param line accumulated Shell::_buffer from Shell::invoke
    void cmd_launch(String line);

// INTERFACE //
    /// draw a new promt intro and reset Shell::_buf_idx
    void ready(void);
    /// add character to buffer and increase Shell::_buf_idx
    void append(char typed);
    /// remove character and decrease Shell::_buf_idx
    /// \param num how much
    void remove(uint16_t num = 1);
    /// accumulate Shell::_buffer and call Shell::cmd_launch
    /// (then Shell::ready)
    void invoke(void);

private:
    Console& _text;                         ///< keep console for in/output here
    const ShellParam _param;                ///< stores current ShellParam

    uint8_t _cmd_idx = 0;                   ///< current enrolled commands position
    Command* _commands;                     ///< store enrolled commands
    uint16_t _buf_idx = 0;                  ///< current input buffer position
    char* _buffer;                          ///< store current input

    uint16_t _launched = 0;                 ///< keep some statistics

public:
    /// get number of launched commands
    uint16_t get_launched(void) { return _launched; }

};

#undef _SHELL_RETURNCODE

#endif
