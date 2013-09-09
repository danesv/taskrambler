// for mmap
#include <sys/mman.h>

// for random
#include <stdlib.h>

// for open and fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// for puts
#include <stdio.h>

// for time
#include <time.h>

int
main(int argc, char * argv[])
{
    struct stat   st;
    char        * map;
    size_t        position;
    char          print_buf[101];
    int           i;

    print_buf[100] = '\0';

    int fd = open("./mmapfiletest.c", O_RDONLY);

    fstat(fd, &st);
    map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    srandom(time(NULL));
    position = random() % (st.st_size - 100);

    for (i=0; i<100; i+=10) {
        print_buf[i+0] = map[position + i + 0];
        print_buf[i+1] = map[position + i + 1];
        print_buf[i+2] = map[position + i + 2];
        print_buf[i+3] = map[position + i + 3];
        print_buf[i+4] = map[position + i + 4];
        print_buf[i+5] = map[position + i + 5];
        print_buf[i+6] = map[position + i + 6];
        print_buf[i+7] = map[position + i + 7];
        print_buf[i+8] = map[position + i + 8];
        print_buf[i+9] = map[position + i + 9];
    }

    puts(print_buf);

    return 0;
}

// vim: set et ts=4 sw=4:
