#include <uuid/uuid.h>
#include <stdio.h>

int
main(int argc, char * argv[])
{
	uuid_t uuid;
	char   uuid_str[37];

	uuid_generate(uuid);
	uuid_unparse(uuid, uuid_str);

	printf("%s\n", uuid_str);

	return 0;
}

// vim: set ts=4 sw=4:
