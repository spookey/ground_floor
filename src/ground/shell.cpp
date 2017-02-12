#include "ground/shell.hpp"

// COMMANDS //
bool Shell::enroll(Shell::Command &cmd) {
    for (uint8_t idx = 0; idx < _cmd_idx; idx++) {
        if (&_commands[idx] == &cmd) { return true; }
    }
    if (_cmd_idx < _param.command_limit) {
        _commands[_cmd_idx] = cmd;
        _cmd_idx++;
        return true;
    }
    return false;
}
// HANDLING //
void Shell::cmd_help(void) {
    _text.log(F("shell"), F("commands"));
    _text.llg(F("enrolled"), String(_cmd_idx));
    _text.llg(F("free"), String(_param.command_limit - _cmd_idx));
    _text.llg(F("launched"), String(_launched));
    if (!_cmd_idx) { return; }
    _text.llg();
    for (uint8_t idx = 0; idx < _cmd_idx; idx++) {
        _text.llg(_commands[idx].name, _commands[idx].help);
    }
    _text.llg(F("anything else"), F("this help"));
}
void Shell::cmd_launch(String line) {
    String name, args = ""; shell_t code;
    int32_t split = line.indexOf(" ");
    if (split > 0) {
        name = line.substring(0, split);
        args = line.substring(split + 1);
    } else { name = line; }
    for (uint8_t idx = 0; idx < _cmd_idx; idx++) {
        if (name == _commands[idx].name) {
            args.trim();
            code = _commands[idx](args);
            _launched++;
            if (code != 0) {
                _text.log(F("error"), name, String(_text.get_filler()), args);
                _text.llg(F("code"), String(code));
                _text.llg(F("text"), cmd_returncode(code));
            }
            return;
        }
    }
    cmd_help();
}
// INTERFACE //
void Shell::ready(void) {
    _buf_idx = 0;
    _text.text(_text.join(
        _text.get_newline(),
        _text.pad(_param.prompt, false, _param.prompt.length() + 1)
    ), false);
}
void Shell::append(char typed) {
    if (_buf_idx < _param.buffer_limit && typed > 31 && typed < 127) {
        _buffer[_buf_idx] = typed;
        _buffer[_buf_idx + 1] = '\0';
        _buf_idx++;
        _text.raw(typed);
        if (_param.double_echo) {
            _text.raw(SHELL_KEY_BACKSP);
            _text.raw(typed);
        }
    }
}
void Shell::remove(uint16_t num) {
    while (num--) {
        if (_buf_idx > 0) {
            _buf_idx--;
            _text.raw(SHELL_KEY_BACKSP);
            if (_param.double_echo) {
                _text.raw(' ');
                _text.raw(SHELL_KEY_BACKSP);
            }
        }
    }
}
void Shell::invoke(void) {
    String line = _buffer;
    line.trim();
    if (_buf_idx > 0 && !!line.length()) {
        cmd_launch(line);
    }
    ready();
}

void Shell::loop(void) {
    char typed = _text.collect();
    switch (typed) {
        case SHELL_KEY_RETURN:          invoke();               break;
        case SHELL_KEY_ESCAPE:          remove(_buf_idx);       break;
        case SHELL_KEY_BACKSP:          remove();               break;
        default:                        append(typed);          break;
    }
}
