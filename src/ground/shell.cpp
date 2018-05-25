#include "ground/shell.hpp"

// COMMANDS //
bool Shell::enroll(Shell::Command &cmd) {
    for (uint8_t idx = 0; idx < this->_cmd_idx; idx++) {
        if (&this->_commands[idx] == &cmd) { return true; }
    }
    if (this->_cmd_idx < this->_param.command_limit) {
        this->_commands[this->_cmd_idx] = cmd;
        this->_cmd_idx++;
        return true;
    }
    return false;
}
// HANDLING //
void Shell::cmd_help(void) {
    this->_text.log(F("shell"), F("commands"));
    this->_text.llg(F("enrolled"), String(this->_cmd_idx));
    this->_text.llg(F("free"), String(
        this->_param.command_limit - this->_cmd_idx
    ));
    this->_text.llg(F("launched"), String(this->_launched));
    if (!this->_cmd_idx) { return; }
    this->_text.llg();
    for (uint8_t idx = 0; idx < this->_cmd_idx; idx++) {
        this->_text.llg(
            this->_commands[idx].name, this->_commands[idx].help
        );
    }
    this->_text.llg(F("anything else"), F("this help"));
}
void Shell::cmd_launch(String line) {
    String name, args = "";
    shell_t code;
    int32_t split = line.indexOf(" ");
    if (split > 0) {
        name = line.substring(0, split);
        args = line.substring(split + 1);
    } else {
        name = line;
    }
    for (uint8_t idx = 0; idx < _cmd_idx; idx++) {
        if (name == this->_commands[idx].name) {
            args.trim();
            code = this->_commands[idx](args);
            this->_launched++;
            if (code != SHELL_OK) {
                this->_text.log(
                    F("error"), name, String(this->_text.get_filler()), args
                );
                this->_text.llg(F("code"), String(code));
                this->_text.llg(F("text"), cmd_returntext(code));
            }
            return;
        }
    }
    this->cmd_help();
}
// INTERFACE //
void Shell::ready(void) {
    this->_buf_idx = 0;
    this->_text.text(this->_text.join(
        this->_text.get_newline(), this->_text.pad(
            this->_param.prompt, false, 1 + this->_param.prompt.length()
        )
    ), false);
}
void Shell::append(char typed) {
    if (
        this->_buf_idx < this->_param.buffer_limit &&
        typed > 31 && typed < 127
    ) {
        this->_buffer[this->_buf_idx] = typed;
        this->_buffer[1 + this->_buf_idx] = '\0';
        this->_buf_idx++;
        this->_text.raw(typed);
        if (this->_param.double_echo) {
            this->_text.raw(SHELL_KEY_BACKSP);
            this->_text.raw(typed);
        }
    }
}
void Shell::remove(uint16_t num) {
    while (num--) {
        if (this->_buf_idx > 0) {
            this->_buffer[this->_buf_idx] = '\0';
            this->_buf_idx--;
            this->_text.raw(SHELL_KEY_BACKSP);
            if (this->_param.double_echo) {
                this->_text.raw(' ');
                this->_text.raw(SHELL_KEY_BACKSP);
            }
        }
    }
}
void Shell::invoke(void) {
    String line = String(this->_buffer);
    line.trim();
    if (this->_buf_idx > 0 && line.length() > 0) {
        this->cmd_launch(line);
    }
    this->ready();
}

void Shell::loop(void) {
    char typed = this->_text.collect();
    switch (typed) {
        case SHELL_KEY_RETURN:  this->invoke();                 break;
        case SHELL_KEY_ESCAPE:  this->remove(this->_buf_idx);   break;
        case SHELL_KEY_BACKSP:  this->remove();                 break;
        default:                this->append(typed);            break;
    }
}
