/* see TextCMD.h */

#include "TextCMD.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Arduino is missing nullptr
#ifndef nullptr
#define nullptr 0
#endif

#ifndef MAX_CMD_LINE_LEN
#define MAX_CMD_LINE_LEN 0x1f
#endif

TextCMD::TextCMD(uint8_t count, cmd_dispatch dispatch[]) {
    _dispatch = dispatch;
    _dispatch_count = count;
    _cmd_line_buf = nullptr;
    argc = 0;
    cur_char = 0;
}

TextCMD::~TextCMD() {
    if (_cmd_line_buf != nullptr) free(_cmd_line_buf);
}

int8_t TextCMD::do_dispatch(const char* line) {
    split_line_to_argv(line);
    return do_dispatch();
}

int8_t TextCMD::add_char(const char ch) {
    int8_t cmd_ret = 0;
    if (_cmd_line_buf == nullptr) {
        _cmd_line_buf = (char *)malloc(MAX_CMD_LINE_LEN);
        _cmd_line_buf[0] = 0;
    }
    int16_t current_line_end = strlen(_cmd_line_buf);
    if (ch == '\b') {
        if (current_line_end) {
            current_line_end--;
        }
    }
    else if (ch == '\r') {
    }
    else if (ch == '\n') {
        if (current_line_end != 0) {
            this->split_line_buf_to_argv();
            cmd_ret = this->do_dispatch();
        }
        current_line_end = 0;
    }
    else {
        if (current_line_end < MAX_CMD_LINE_LEN) {
            _cmd_line_buf[current_line_end++] = ch;
        }
    }
    if (_cmd_line_buf != nullptr) { _cmd_line_buf[current_line_end] = '\0'; }
    cur_char = ch;
    return cmd_ret;
}

int8_t TextCMD::do_dispatch() {
    if (!argc) {
        return 0;
    }
    int8_t ret = -1;
    for (uint8_t i = 0; i < _dispatch_count; i++) {
        if (strcmp(_dispatch[i].cmd, argv[0]) == 0) {
            ret = _dispatch[i].cb(argc,(const char**)argv);
            break;
        }
    }
    free(_cmd_line_buf);
    _cmd_line_buf = nullptr;
    return ret;
}

void TextCMD::set_argv(uint8_t new_argc, char* new_argv[]) {
    argc = new_argc;
    for (uint8_t i = 0; i < argc; i++) {
        argv[i] = new_argv[i];
    }
}

void TextCMD::split_line_to_argv(const char* input_line) {
    if (_cmd_line_buf != nullptr) free(_cmd_line_buf);
    int line_len = strlen(input_line);
    _cmd_line_buf = (char *)malloc(line_len);
    strcpy(_cmd_line_buf, input_line);
    split_line_buf_to_argv();
}

void TextCMD::split_line_buf_to_argv() {
    argc = 0;
    bool new_arg = true;
    uint8_t args_start = 0;
    int line_len = strlen(_cmd_line_buf);

    // cmds with spaces inside as argv[0]
    for (uint8_t i = 0; i < _dispatch_count; i++) {
        uint8_t cmd_len = strlen(_dispatch[i].cmd);
        if (
            (strncmp(_cmd_line_buf,_dispatch[i].cmd,cmd_len) == 0)
            && ((_cmd_line_buf[cmd_len] == ' ') || (_cmd_line_buf[cmd_len] == '\0'))
        ) {
            _cmd_line_buf[cmd_len] = '\0';
            argv[0] = &_cmd_line_buf[0];
            argc = 1;
            args_start = cmd_len + 1;
            break;
        }
    }

    for (int i = args_start; i < line_len; i++) {
        if (_cmd_line_buf[i] == ' ') {
            new_arg = true;
            _cmd_line_buf[i] = '\0';
            continue;
        }
        if (new_arg) {
            if (argc+1 == TextCMD_max_argv) {
                break;
            }
            argv[argc] = &_cmd_line_buf[i];
            argc++;
            new_arg = false;
        }
    }
}

