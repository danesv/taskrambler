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

    /*
     * I think this has something like O(n+log(n)) on a ballanced
     * tree because I have to traverse back the rightmost leaf to
     * the root to get a break condition.
     */
    while (node) {
        /*
         * If we come from the right so nothing and go to our
         * next parent.
         */
        if (previous == node->right) {
            previous = node;
            node     = node->parent;
            depth--;
            continue;
        }

        if ((NULL == node->left || previous == node->left)) {
            /*
             * If there are no more elements to the left or we
             * came from the left, process data.
             */
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
            /*
             * if there are more elements to the left go there.
             */
            previous = node;
            node     = node->left;
            depth++;
        }
    }
}

void printElement(int data, int depth)
{
    int  i;

    printf("%02d(%02d)", data, depth);
    for (i=0; i<depth; i++) printf("-");
    puts("");
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
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
     * delete does not work correctly here..
     * luckily I do not need the simple binary trees anymore
     * as I have rbtrees.
     */
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 8);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 11);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 13);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 3);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 16);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 10);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 9);
    puts("traverse");
    traverse(root, printElement);

    deleteElement(&root, 12);
    puts("traverse");
    traverse(root, printElement);

    return 0;
}

// vim: set et ts=4 sw=4:
