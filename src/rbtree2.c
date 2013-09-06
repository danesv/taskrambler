#include <stdio.h>

#include "class.h"
#include "commons.h"
#include "utils/memory.h"

#include "tree.h"

#define NVALUES  10

int
insertCompare(const void * tval, const void * search)
{
    return *(const int *)tval - *(const int *)search;
}

void
printNode(const void * _node, const int depth)
{
    //Tree node  = (Tree)_node;
    //int  value = *(int *)node->data;
    int  value = *(int *)_node;
    int  i;

    printf("%010d(%02d)", value, depth);

    // printf("%s %010d(%02d)",
    //         (node->color==rbRed)?"R":"B",
    //         value,
    //         depth);
    for (i=0; i<depth; i++) printf("-");
    puts("");
}


/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    Tree  root     = NULL;
    int * found    = NULL;
    int   values[] = {40, 50, 60, 70, 80, 45, 75, 85};
    int   new50    = 50;
    int   new70    = 70;
    int   new80    = 80;
    int   search10 = 10;
    int   search64 = 64;

    treeInsert(&root, (void *)&(values[0]), insertCompare);
    treeInsert(&root, (void *)&(values[1]), insertCompare);
    treeInsert(&root, (void *)&(values[2]), insertCompare);
    treeInsert(&root, (void *)&(values[3]), insertCompare);
    treeInsert(&root, (void *)&(values[4]), insertCompare);
    treeInsert(&root, (void *)&(values[5]), insertCompare);
    treeInsert(&root, (void *)&(values[6]), insertCompare);
    treeInsert(&root, (void *)&(values[7]), insertCompare);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeInsert(&root, (void *)&new70, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &new70, new70, found, *found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeFind(root, &search10, insertCompare);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 10);
    } else {
        printf("found %d\n", *found);
    }
    puts("");

    found = treeFind(root, &search64, insertCompare);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 64);
    } else {
        printf("found %d\n", *found);
    }
    puts("");

    found = treeFind(root, &new70, insertCompare);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 70);
    } else {
        printf("found %d\n", *found);
    }
    puts("");

    found = treeDelete(&root, (void *)&new70, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &new70, new70, found, *found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeInsert(&root, (void *)&new80, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &new80, new80, found, *found);
    found = treeInsert(&root, (void *)&new50, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &new50, new50, found, *found);
    found = treeInsert(&root, (void *)&new80, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &new80, new80, found, *found);

    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&new80, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &new80, new80, found, *found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&new50, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &new50, new50, found, *found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&new70, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &new70, new70, found, found?*found:-1);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&(values[2]), insertCompare);
    printf("delete %p(%d) got %p(%d)\n",
            &(values[2]),
            values[2],
            found,
            found?*found:-1);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

//
//    post(root, cleanup);

    return 0;
}

// vim: set et ts=4 sw=4:
