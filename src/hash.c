#include <stdio.h>
#include <stdlib.h>

#include "class.h"
#include "commons.h"
#include "utils/memory.h"

#include "hash.h"
#include "utils/memory.h"


/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    Hash      hash = new(Hash);
    HashValue deleted;

    hashAdd(hash, new(HashValue, CSTRA("foo"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("hjkfdd"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("j8frheff"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("f9e0g"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("gfrk9e0"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("fr09ie"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("fu8de9"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("rehw78"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("fcrne9"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("new8"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("fdhe78"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("dhew8"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("jfde9w8"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("dhe7w89"), CSTRA("bar")));
    hashAdd(hash, new(HashValue, CSTRA("fduew89"), CSTRA("bar")));

    deleted = hashDelete(hash, CSTRA("f9e0g"));
    delete(deleted);
    //deleted = hashDelete(hash, CSTRA("fcrne9"));
    //delete(deleted);
    //deleted = hashDelete(hash, CSTRA("fr09ie"));
    //delete(deleted);
    //deleted = hashDelete(hash, CSTRA("jfde9w8"));
    //delete(deleted);
    //deleted = hashDelete(hash, CSTRA("j8frheff"));
    //delete(deleted);

    delete(hash);
    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
