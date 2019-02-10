/* VERSION 0.01; 29.12.2017; see below for description and documentation */
#ifndef TextCMD_h
#define TextCMD_h

#include <stdint.h>

const uint8_t TextCMD_max_argv = 10;

struct cmd_dispatch {
    const char* cmd;
    int8_t (*cb)(uint8_t argc, const char* argv[]);
};

class TextCMD {
    public:
        TextCMD(uint8_t count, cmd_dispatch dispatch[]);
        ~TextCMD();
        uint8_t argc = 0;
        char* argv[TextCMD_max_argv];
        void split_line_to_argv(const char* line);
        void set_argv(uint8_t new_argc, char* new_argv[]);
        int8_t do_dispatch();
        int8_t do_dispatch(const char* line);
        // int argv_to_int(uint8_t idx);    # TODO parse argv[idx-1] and return integer of that argument
    private:
        uint8_t _dispatch_count = 0;
        char* _cmd_line_buf;
        cmd_dispatch* _dispatch;
};

#endif

/*

=head1 NAME

TextCMD.h - Arduino library for decode text commands

=head1 SYNOPSIS

    #include <Arduino.h>
    #include <TextCMD.h>            // https://github.com/jozef/Arduino-TextCMD

    cmd_dispatch commands[] = {
        { "?", &cmd_help     },
        { "*", &cmd_multiply }
    };
    TextCMD cmd((sizeof(commands)/sizeof(commands[0])),commands);

    void setup () {
        Serial.begin(9600);
    }

    void loop () {
        cmd.do_dispatch("?");
        cmd.do_dispatch("* 5 5");

        delay(5000);
    }

    int8_t cmd_help(uint8_t argc, char* argv[]) {
        Serial.println("supported commands:");
        Serial.println("    * [int] [int]    - multiply two number");
        Serial.println("    help/?          - print this help");
        return 0;
    }
    int8_t cmd_multiply(uint8_t argc, char* argv[]) {
        if (argc != 3) return -1;
        Serial.println(
            String(argv[1]) + " * " + String(argv[2]) + " = "
            + String(String(argv[1]).toInt()*String(argv[2]).toInt()));
        return 0;
    }

=head1 DESCRIPTION

Helps to parse text lines and execute associated functions.

=head1 LICENSE

This is free software, licensed under the MIT License.

=head1 AUTHOR

    Jozef Kutej

=cut

*/
