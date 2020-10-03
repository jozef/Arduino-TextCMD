/* VERSION 0.01; 2019-02-10; see below for description and documentation */
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
        uint8_t argc;
        char cur_char;
        char* argv[TextCMD_max_argv];
        void split_line_to_argv(const char* line);
        void set_argv(uint8_t new_argc, char* new_argv[]);
        int8_t do_dispatch();
        int8_t do_dispatch(const char* line);
        int8_t add_char(const char);
        uint8_t cur_len();
    private:
        uint8_t _dispatch_count = 0;
        char* _cmd_line_buf;
        cmd_dispatch* _dispatch;
        void split_line_buf_to_argv();
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
        cmd.do_dispatch("?");
        cmd.do_dispatch("* 5 5");
    }

    void loop () {
        while (Serial.available()) {
            char ch = Serial.read();
            if (ch == '\b') { Serial.print(F("\b \b")); }
            else { Serial.print(ch); }
            switch (cmd.add_char(ch)) {
                case -1: Serial.println(F("unknown command or syntax error. send '?' for help")); break;
            }
        }
        delay(100);
    }

    int8_t cmd_help(uint8_t argc, const char* argv[]) {
        Serial.println(F("supported commands:"));
        Serial.println(F("    * [int] [int]    - multiply two number"));
        Serial.println(F("    ?                - print this help"));
        return 0;
    }
    int8_t cmd_multiply(uint8_t argc, const char* argv[]) {
        if (argc != 3) return -1;
        Serial.println(
            String(argv[1]) + " * " + String(argv[2]) + " = "
            + String(String(argv[1]).toInt()*String(argv[2]).toInt()));
        return 0;
    }

=head1 DESCRIPTION

Helps to parse text command lines and execute associated functions.

Constructor needs a list of commands and associated functions. Command is
a C<char*> and function pointers is of type:

    int8_t (*cb)(uint8_t argc, const char* argv[])

These callback function will get number of arguments and an array of these
arguments as parameter. Return value is a 8bit integer. Function can return
any value, -1 is returned by this code when there is no matching callback
function found.

=head1 METHODS

=head2 int8_t do_dispatch(const char* line);

C<char* line> passed as parameter will be split into arguments and the first
will be matched against the list of commands. Function of the first matching
command will be executed and its return value returned. Otherwise returns -1.

=head2 int8_t add_char(const char);

Similar as C<do_dispatch()> but it keeps on adding charaters to a line buffer
until new-line is reached. At that point C< do_dispatch() > is triggered.
Returns 0 in all cases but when new-line is sent, in which case it returns
C< do_dispatch() > return value.

See L</SYNOPSIS> or F<examples/TextCMD_synopsis/TextCMD_synopsis.ino> for
a full C<add_char()> example of reading and echoing input from serial input.

=head1 LICENSE

This is free software, licensed under the MIT License.

=head1 AUTHOR

    Jozef Kutej

=cut

*/
