#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
#include <errno.h>
#include <string.h>

#include "class.h"
#include "commons.h"
#include "user.h"
#include "storage.h"

#include "utils/memory.h"

int
main(int argc, char * argv[])
{
    Storage users = new(Storage, CSTRA("./users.db"));
    User    user;
    User    georg;
    //User    steffi;
    size_t  nuser;

    if (NULL == users) {
        fprintf(stderr, "%s\n", gdbm_strerror(gdbm_errno));
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    georg = new(User,
            CSTRA("georg@steffers.org"),
            CSTRA("Georg"),
            CSTRA("Hopp"));

    userSave(georg, users);
    delete(georg);

    user = new(User, NULL);
    user->email  = "georg@steffers.org";
    nuser        = sizeof("georg@steffers.org") - 1;
    user->nemail = &nuser;

    if (NULL == userLoad(user, users)) {
        fprintf(stderr, "can't find user");
    } else {
        puts("found user:");
        puts(user->email);
        puts(user->firstname);
        puts(user->surname);
    }

    delete(user);
    delete(users);

    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
