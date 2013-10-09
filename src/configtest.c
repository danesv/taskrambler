#include <stdio.h>

#include "class.h"
#include "commons.h"
#include "config/config.h"
#include "config/value.h"
#include "utils/memory.h"

int
main(int argc, char * argv[])
{
    Config      config = new(Config, "./testconfig.cfg");
    ConfigValue val;

    val = configGet(config, CSTRA("dummy"));

    if (NULL != val) {
        switch (val->type) {
            case CONFIG_VALUE_STRING:
                printf(
                        "Value for dummy: (STRING): %s\n",
                        (val->value).string);
                break;

            case CONFIG_VALUE_NUMERIC:
                printf(
                        "Value for dummy: (NUMERIC): %lld\n",
                        (val->value).number);
                break;

            default:
                printf("Invalid config...that should never happen\n");
        }
    }

    delete(config);
    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
