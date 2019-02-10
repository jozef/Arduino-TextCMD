#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <TextCMD.h>

using namespace std;

int8_t cmd_feedback = 0;

void ok(bool test, const char *name) {
    printf("%sok - %s\n", (test ? "" : "not "), name);
}

int8_t cmd_help(uint8_t argc, const char* argv[]) {
    cmd_feedback = 1;
    ok(argc == 1, "one argument, command it self to cmd_help()");
    return 0;
}

int8_t cmd_math_op(uint8_t argc, const char* argv[]) {
    if (argc != 3) return -2;
    int num1 = stoi(argv[1]);
    int num2 = stoi(argv[2]);
    if (argv[0][0] == '+') return cmd_feedback = num1 + num2;
    if (argv[0][0] == '-') return cmd_feedback = num1 - num2;
    if (argv[0][0] == '*') return cmd_feedback = num1 * num2;
    if (strcmp(argv[0], "div") == 0) return cmd_feedback = num1 / num2;
    return 0;
}

void test_do_dispatch(TextCMD* cmd) {
    ok(cmd->do_dispatch("?") == 0, "dispatch known command");
    ok(cmd_feedback == 1, "cmd_help called");
    ok(cmd->do_dispatch("unknown") == -1, "unknown command returns -1");
    cmd->do_dispatch("+ 5 5");
    ok(cmd_feedback == 10, "cmd_math_op called, addition");
    cmd->do_dispatch("- 5 2");
    ok(cmd_feedback == 3, "cmd_math_op called, subtraction");
    cmd->do_dispatch("* 5 2");
    ok(cmd_feedback == 10, "cmd_math_op called, multiplication");
    cmd->do_dispatch("div 5 2");
    ok(cmd_feedback == 2, "cmd_math_op called, division");
    ok(cmd->do_dispatch("+ 5 2 3") == -2, "on wrong number of parameters");
}

int main(int argc, char* argv[]) {
    cmd_dispatch commands[] = {
        { "?", &cmd_help    },
        { "+", &cmd_math_op },
        { "-", &cmd_math_op },
        { "*", &cmd_math_op },
        { "div", &cmd_math_op }
    };
    TextCMD cmd((sizeof(commands)/sizeof(commands[0])),commands);

    test_do_dispatch(&cmd);

    return 0;
}
