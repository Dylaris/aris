#define COOK_IMPLEMENTATION
#include "cook.h"

int main(void) {
    cook_cmd_t cmd = {0};

    cook_cmd_append_many(&cmd, "gcc", "--version");
    cook_cmd_run(&cmd);
    cook_cmd_reset(&cmd);

    cook_cmd_append(&cmd, "gcc %s", __FILE__);
    cook_cmd_append(&cmd, "-I..");
    cook_cmd_run(&cmd);
    cook_cmd_reset(&cmd);

    cook_cmd_free(&cmd);

    return 0;
}
