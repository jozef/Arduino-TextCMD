/*
=head1 DESCRIPTION

synopsis section example from TextCMD.h

=cut
*/

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
