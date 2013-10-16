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

void
printUser(Storage users, const char * key, size_t nkey)
{
    User user = new(User, NULL);

    user->email  = (char *)key,
    user->nemail = &nkey;

    if (NULL == userLoad(user, users)) {
        fprintf(stderr, "can't find user: %s\n", key);
    } else {
        puts("found user:");
        puts(user->email);
        puts(user->firstname);
        puts(user->surname);

        delete(user);
    }
}

void
insertUser(
        Storage users,
        const char * email, size_t nemail,
        const char * firstname, size_t nfirstname,
        const char * surname, size_t nsurname)
{
    User insert = new(User,
            email, nemail,
            firstname, nfirstname,
            surname, nsurname);

    userSave(insert, users);
    delete(insert);
}

int
main(int argc, char * argv[])
{
    Storage users = new(Storage, CSTRA("./users.db"));

    if (NULL == users) {
        fprintf(stderr, "%s\n", gdbm_strerror(gdbm_errno));
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }

    insertUser(
            users,
            CSTRA("georg"),
            CSTRA("Georg"),
            CSTRA("Hopp"));
    insertUser(
            users,
            CSTRA("georg@steffers.org"),
            CSTRA("Georg"),
            CSTRA("Hopp"));
    insertUser(
            users,
            CSTRA("drachenfrau1982@gmx.net"),
            CSTRA("Gundula"),
            CSTRA("Hopp"));

    printUser(users, CSTRA("foo@bar.de"));
    puts("");
    printUser(users, CSTRA("drachenfrau1982@gmx.net"));
    puts("");
    printUser(users, CSTRA("georg@steffers.org"));

    delete(users);

    users = new(Storage, CSTRA("./users.db"));

    insertUser(
            users,
            CSTRA("drachenfrau1982@gmx.net"),
            CSTRA("Stefanie"),
            CSTRA("Hopp"));

    puts("");
    printUser(users, CSTRA("foo@bar.de"));
    puts("");
    printUser(users, CSTRA("drachenfrau1982@gmx.net"));
    puts("");
    printUser(users, CSTRA("georg@steffers.org"));

    delete(users);

    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
