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
    return -2;
}

int8_t cmd_math_op(uint8_t argc, const char* argv[]) {
    if (argc != 3) return -2;
    int num1 = stoi(argv[1]);
    int num2 = stoi(argv[2]);
    if (argv[0][0] == '+') return cmd_feedback = num1 + num2;
    if (argv[0][0] == '-') return cmd_feedback = num1 - num2;
    if (argv[0][0] == '*') return cmd_feedback = num1 * num2;
    if (strcmp(argv[0], "div") == 0) return cmd_feedback = num1 / num2;
    return -2;
}

void test_do_dispatch(TextCMD* cmd) {
    cmd_feedback = 0;
    ok(cmd->do_dispatch("?") == -2, "dispatch known command");
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

void test_add_char(TextCMD* cmd) {
    cmd_feedback = 0;
    ok(cmd->add_char('?') == 0, "add char");
    ok(cmd->add_char('\n') == -2, "add char end of line -> cmd_help");
    ok(cmd_feedback == 1, "cmd_help called");
    ok(cmd->add_char('x') == 0, "add char");
    ok(cmd->add_char('\n') == -1, "add char; end of line -> unknown command");
    cmd->add_char('*');
    cmd->add_char(' ');
    cmd->add_char('2');
    cmd->add_char(' ');
    cmd->add_char('3');
    cmd->add_char('\n');
    ok(cmd_feedback == 6, "cmd_math_op called, multiplication");
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
    test_add_char(&cmd);

    return 0;
}
