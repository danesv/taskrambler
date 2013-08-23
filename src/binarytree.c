#include <stdio.h>
#include <stdlib.h>


struct element
{
    int data;

    struct element * parent;
    struct element * left;
    struct element * right;
};

struct element *
newElement(int data)
{
    struct element * el = malloc(sizeof(struct element));
    el->data   = data;
    el->parent = NULL;
    el->left   = NULL;
    el->right  = NULL;

    return el;
}

/**
 * find element in tree
 */
struct element *
findElement(struct element * tree, int data)
{
    while (NULL != tree) {
        if (tree->data == data) {
            break;
        }

        if (data < tree->data) {
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }

    return tree;
}

/**
 * insert element in tree
 */
void
insertElement(struct element ** tree, int data)
{
    struct element * node = *tree;

    if (NULL == node) {
        *tree = newElement(data);
        return;
    }

    while (data != node->data) {
        if (data < node->data) {
            if (NULL == node->left) {
                node->left         = newElement(data);
                node->left->parent = node;
                return;
            } else {
                node = node->left;
            }
        } else {
            if (NULL == node->right) {
                node->right         = newElement(data);
                node->right->parent = node;
                return;
            } else {
                node = node->right;
            }
        }
    }
}

/**
 * delete element from tree
 * here multiple functions are involved....
 * =======================================================================
 */
/**
 * find minimum of the right subtree aka leftmost leaf of right subtree
 * aka left in-order successor.
 * We return the parent of the element in the out argument parent.
 * This can be NULL wenn calling.
 */
struct element *
findInOrderSuccessor(struct element * tree)
{
    struct element * node = tree->right;

    while (NULL != node->left) {
        node = node->left;
    }

    return node;
}

void
deleteElement(struct element ** tree, int data)
{
    struct element * node   = *tree;

    // find the relevant node and it's parent
    while (NULL != node && node->data != data) {
        if (data < node->data) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    // element not found
    if (NULL == node) {
        return;
    }

    // distinuish 3 cases, where the resolving of each case leads to the
    // precondition of the other.

    // case 1: two children
    if (NULL != node->left && NULL != node->right) {
        struct element * successor = findInOrderSuccessor(node);

        node->data = successor->data;
        node       = successor;
    }

    // case 2: one child wither left or right
    if (NULL != node->left) {
        //node->data = node->left->data;
        //node       = node->left;
        if (NULL != node->parent) {
            if (node == node->parent->left) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
        }
        node->left->parent = node->parent;
    } 

    if (NULL != node->right) {
        //node->data = node->right->data;
        //node       = node->right;
        if (NULL != node->parent) {
            if (node == node->parent->left) {
                node->parent->left = node->right;
            } else {
                node->parent->right = node->right;
            }
        }
        node->right->parent = node->parent;
    }

    // case 3: we are a leaf
    if (NULL != node->parent) {
        if (node == node->parent->left) {
            node->parent->left  = NULL;
        } else {
            node->parent->right = NULL;
        }
    }

    if (node == *tree) {
        if (NULL != node->left) {
            *tree = node->left;
        } else if (NULL != node->right) {
            *tree = node->right;
        } else {
            *tree = NULL;
        }
    }

    free(node);
}


void
traverse(struct element * tree, void (*cb)(int, int))
{
    struct element * previous = tree;
    struct element * node     = tree;
    int    depth              = 1;

    // I think this has something like O(n+log(n)) on a ballanced
    // tree because I have to traverse back the rightmost leaf to
    // the root to get a break condition.
    while (node) {
        if (previous == node->right) {
            previous = node;
            node     = node->parent;
            depth--;
            continue;
        }

        if ((NULL == node->left || previous == node->left)) {
            cb(node->data, depth);
            previous = node;

            if (NULL != node->right) {
                node = node->right;
                depth++;
            } else {
                node = node->parent;
                depth--;
            }
        } else {
            previous = node;
            node     = node->left;
            depth++;
        }
    }
}

void printElement(int data, int depth)
{
    int  i;

    for (i=0; i<depth-1; i++) printf("-");
    printf("%02d(%02d)\n", data, depth);
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    int              i;
    struct element * root = NULL;

    insertElement(&root, 13);
    insertElement(&root, 8);
    insertElement(&root, 16);
    insertElement(&root, 11);
    insertElement(&root, 3);
    insertElement(&root, 9);
    insertElement(&root, 12);
    insertElement(&root, 10);

    /*
     * after this I have the following:
     *
     * Element 03: n=0x0xcf50d0 p=0x0xcf5040 l=0x(nil) r=0x(nil)
     * Element 08: n=0x0xcf5040 p=0x0xcf5010 l=0x0xcf50d0 r=0x0xcf50a0
     * Element 09: n=0x0xcf5100 p=0x0xcf50a0 l=0x(nil) r=0x0xcf5160
     * Element 10: n=0x0xcf5160 p=0x0xcf5100 l=0x(nil) r=0x(nil)
     * Element 11: n=0x0xcf50a0 p=0x0xcf5040 l=0x0xcf5100 r=0x0xcf5130
     * Element 12: n=0x0xcf5130 p=0x0xcf50a0 l=0x(nil) r=0x(nil)
     * Element 13: n=0x0xcf5010 p=0x(nil) l=0x0xcf5040 r=0x0xcf5070
     * Element 16: n=0x0xcf5070 p=0x0xcf5010 l=0x(nil) r=0x(nil)
     *
     * which translates to:
     * 
     * 03 has p:08, l:N , R:N
     * 08 has p:13, l:03, r:11
     * 09 has p:11, l:N , r:10
     * 10 has p:09, l:N , r:N
     * 11 has p:08, l:09, r:12
     * 12 has p:11, l:N , r:N
     * 13 has p:N , l:08, r:16
     * 16 has p:13, l:N , r:N
     *                    
     * which visualizes as:
     *                      13
     *           08                      16
     *      03            11            0  0
     *     0  0       09      12
     *              0   10   0  0
     *
     * Looks like the insert works properly.
     * So the problem is out traversing...
     */
    puts("elements:");
    for (i=1; i<20; i++) {
        struct element * element = findElement(root, i);
        printf("Element %02d: n=0x%p p=0x%p l=0x%p r=0x%p\n",
                i,
                element,
                element ? element->parent : 0x0,
                element ? element->left : 0x0,
                element ? element->right : 0x0);
    }

    puts("traverse");
    traverse(root, printElement);

    return 0;
}

// vim: set et ts=4 sw=4:
