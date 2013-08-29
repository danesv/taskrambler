// for random
#include <stdlib.h>

// for puts
#include <stdio.h>

// for time
#include <time.h>

#include "class.h"
#include "commons.h"
#include "utils/memory.h"

#include "asset.h"

int
main(int argc, char * argv[])
{
    size_t i;
    size_t position;
    char   print_buf[101];

    Asset  asset = new(Asset, CSTRA("./src/mmapfiletest.c"));

    print_buf[100] = '\0';

    srandom(time(NULL));
    position = random() % (asset->size - 100);

    for (i=0; i<100; i+=10) {
        print_buf[i+0] = asset->data[position+i+0];
        print_buf[i+1] = asset->data[position+i+1];
        print_buf[i+2] = asset->data[position+i+2];
        print_buf[i+3] = asset->data[position+i+3];
        print_buf[i+4] = asset->data[position+i+4];
        print_buf[i+5] = asset->data[position+i+5];
        print_buf[i+6] = asset->data[position+i+6];
        print_buf[i+7] = asset->data[position+i+7];
        print_buf[i+8] = asset->data[position+i+8];
        print_buf[i+9] = asset->data[position+i+9];
    }

    if (NULL != asset->mime_type) {
        puts(asset->mime_type);
    }
    puts(print_buf);

    delete(asset);
    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
