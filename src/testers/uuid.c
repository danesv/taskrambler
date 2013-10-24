#include <stdio.h>
#include <uuid/uuid.h>

#include "class.h"
#include "uuid.h"

#include "commons.h"
#include "utils/memory.h"


#define UUID_NS "4f947b70-6f9f-43b6-8dc1-1321977c8240"


int
main(int argc, char * argv[])
{
    UuidString uuid_str;

    Uuid nsid = uuidParse(UUID_NS);
    Uuid ver1 = uuidVersion1();
    Uuid ver3 = uuidVersion3("foo", 3, nsid);
    Uuid ver5 = uuidVersion3("foo", 5, nsid);

    uuidUnparse(nsid, uuid_str);
    printf("nsid: %s\n", uuid_str);

    uuidUnparse(ver1, uuid_str);
    printf("nsid: %s\n", uuid_str);

    uuidUnparse(ver3, uuid_str);
    printf("nsid: %s\n", uuid_str);

    uuidUnparse(ver5, uuid_str);
    printf("nsid: %s\n", uuid_str);

    delete(ver5);
    delete(ver3);
    delete(ver1);
    delete(nsid);

    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
