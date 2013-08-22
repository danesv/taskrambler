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
    struct element ** node = tree;

    if (NULL == *node) {
        *node = newElement(data);
        return;
    }

    while (data != (*node)->data) {
        if (data < (*node)->data) {
            if (NULL == (*node)->left) {
                (*node)->left         = newElement(data);
                (*node)->left->parent = *node;
                return;
            } else {
                *node = (*node)->left;
            }
        } else {
            if (NULL == (*node)->right) {
                (*node)->right         = newElement(data);
                (*node)->right->parent = *node;
                return;
            } else {
                *node = (*node)->right;
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
    struct element * parent = NULL;
    struct element * node   = *tree;

    // find the relevant node and it's parent
    while (NULL != node && node->data != data) {
        parent = node;

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
        parent     = successor->parent;
        node       = successor;
    }

    // case 2: one child wither left or right
    if (NULL != node->left) {
        node->data = node->left->data;
        parent     = node;
        node       = parent->left;
    } 

    if (NULL != node->right) {
        node->data = node->right->data;
        parent     = node;
        node       = parent->right;
    }

    // case 3: we are a leaf
    if (NULL != parent) {
        if (node == parent->left) {
            parent->left  = NULL;
        } else {
            parent->right = NULL;
        }
    }

    free(node);
    if (node == *tree) {
        *tree = NULL;
    }
}


void
traverse(struct element * tree, void (*cb)(int, int))
{
    struct element * previous = NULL;
    struct element * node     = tree;
    int    depth              = 1;

    while (tree) {
        if ((NULL == node->left || previous == node->left) &&
                previous != node->right) {

            cb(node->data, depth);
            previous = node;

            if (NULL != node->right) {
                node = node->right;
                depth++;
            } else {
                if (node->parent->right == node) {
                    break;
                } 

                node = node->parent;
                depth--;
            }
        } else {
            previous = node;

            if (previous == node->left || previous == node->right) {
                node = node->parent;
                depth--;
            } else {
                node = node->left;
                depth++;
            }
        }
    }
}

void printElement(int data, int depth)
{
    char format[250];

    sprintf(format, "%% %dd(%%d)\n", depth * 3);
    printf(format, data, depth);
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    struct element * root = NULL;

    puts ("insert 5:\n");
    insertElement(&root, 5);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("insert 4:\n");
    insertElement(&root, 4);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("insert 5:\n");
    insertElement(&root, 5);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("insert 6:\n");
    insertElement(&root, 6);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("traverse");
    traverse(root, printElement);
    puts ("\n");

    puts ("delete 5 (one child on both sides):\n");
    deleteElement(&root, 5);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("delete 6 (one child on the left):\n");
    deleteElement(&root, 6);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("insert 6:\n");
    insertElement(&root, 6);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("delete 6 (a leaf):\n");
    deleteElement(&root, 6);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    puts ("delete 4 (a leaf and root):\n");
    deleteElement(&root, 4);
    printf("R: 0x%p\n", root);
    printf("4: 0x%p\n", findElement(root, 4));
    printf("5: 0x%p\n", findElement(root, 5));
    printf("6: 0x%p\n\n", findElement(root, 6));

    return 0;
}

// vim: set et ts=4 sw=4:
