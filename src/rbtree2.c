#include <stdio.h>
#include <stdlib.h>

#include "class.h"
#include "commons.h"
#include "utils/memory.h"

#include "tree.h"
#include "utils/memory.h"

#define NVALUES  10

int
insertCompare(const void * tval, const void * search)
{
    return *(const int *)tval - *(const int *)search;
}

void
freeNode(const void * data, const int depth)
{
    printf("now free %d at %p\n", *(int*)data, data);
    MEM_FREE(data);
}

void
printNode(const void * _node, const int depth)
{
    Tree node  = (Tree)_node;
    int  value = *(int *)node->data;
    int  i;

    for (i=1; i<7; i++) i<=depth?printf("-"):printf(" ");
    printf("%p:%d p:%p l:%p r:%p\n",
            node, value, node->parent, node->left, node->right);

    // printf("%s %010d(%02d)",
    //         (node->color==rbRed)?"R":"B",
    //         value,
    //         depth);
}


void *
newEle(int value)
{
    void * val = memMalloc(sizeof(int));

    *(int*)val = value;
    return val;
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    Tree  root    = NULL;
    int * found   = NULL;
    int * element = NULL;

    int search10 = 10;
    int search64 = 64;
    int search70 = 70;
    int search80 = 80;
    int search50 = 50;

    treeInsert(&root, newEle(40), insertCompare);
    treeInsert(&root, newEle(50), insertCompare);
    treeInsert(&root, newEle(60), insertCompare);
    treeInsert(&root, newEle(70), insertCompare);
    treeInsert(&root, newEle(80), insertCompare);
    treeInsert(&root, newEle(45), insertCompare);
    treeInsert(&root, newEle(75), insertCompare);
    treeInsert(&root, newEle(85), insertCompare);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    element = newEle(70);
    found = treeInsert(&root, element, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", element, *element, found, *found);
    if (found != element) {
        printf("remove duplicate");
        MEM_FREE(element);
    }
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

    found = treeFind(root, &search70, insertCompare);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 70);
    } else {
        printf("found %d\n", *found);
    }
    puts("");

    found = treeDelete(&root, (void *)&search70, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &search70, search70, found, *found);
    MEM_FREE(found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeInsert(&root, (void *)&search80, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &search80, search80, found, *found);
    found = treeInsert(&root, (void *)&search50, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &search50, search50, found, *found);
    found = treeInsert(&root, (void *)&search80, insertCompare);
    printf("insert %p(%d) got %p(%d)\n", &search80, search80, found, *found);

    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&search80, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &search80, search80, found, *found);
    MEM_FREE(found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&search50, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &search50, search50, found, *found);
    MEM_FREE(found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    found = treeDelete(&root, (void *)&search70, insertCompare);
    printf("delete %p(%d) got %p(%d)\n", &search70, search70, found, found?*found:-1);
    MEM_FREE(found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    element = newEle(60);
    found   = treeDelete(&root, element, insertCompare);
    printf("delete %p(%d) got %p(%d)\n",
            element,
            *element,
            found,
            found?*found:-1);
    if (found != element) {
        MEM_FREE(element);
    }
    MEM_FREE(found);
    puts("traverse");
    treeWalk(root, printNode);
    puts("");

    treeDestroy(&root, freeNode);
    memCleanup();

    return 0;
}

// vim: set et ts=4 sw=4:
