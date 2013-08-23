#include <stdio.h>
#include <stdlib.h>


enum rbColor {rbBlack=1, rbRed=2};


struct element
{
    int data;

    enum rbColor color;

    struct element * parent;
    struct element * left;
    struct element * right;
};

struct element *
newElement(int data)
{
    struct element * node = malloc(sizeof(struct element));
    node->data   = data;
    node->color  = rbRed;
    node->parent = NULL;
    node->left   = NULL;
    node->right  = NULL;

    return node;
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
struct element *
insertElement(struct element ** tree, int data)
{
    struct element * node = *tree;

    if (NULL == node) {
        *tree = newElement(data);
        return *tree;
    }

    while (data != node->data) {
        if (data < node->data) {
            if (NULL == node->left) {
                node->left         = newElement(data);
                node->left->parent = node;
                return node->left;
            } else {
                node = node->left;
            }
        } else {
            if (NULL == node->right) {
                node->right         = newElement(data);
                node->right->parent = node;
                return node->right;
            } else {
                node = node->right;
            }
        }
    }

    return NULL;
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


/*
 * rbinsert from wikipedia...see later if this could be
 * optiized.
 */
struct element *
grandparent(struct element * node)
{
    if (NULL != node && NULL != node->parent) {
        return node->parent->parent;
    }

    return NULL;
}

struct element *
uncle(struct element * node)
{
    struct element * gp = grandparent(node);

    if (NULL == gp) {
        return NULL;
    }

    if (node->parent == gp->left) {
        return gp->right;
    }

    return gp->left;
}

void
rotateLeft(struct element ** tree, struct element * node)
{
    struct element * rightChild = node->right;
    struct element * rcLeftSub  = node->right->left;

    rightChild->left   = node;
    rightChild->parent = node->parent;
    node->right        = rcLeftSub;
    rcLeftSub->parent  = node;

    if (node->parent) {
        if (node->parent->left == node) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }
    } else {
        *tree = rightChild;
    }

    node->parent = rightChild;
}

void
rotateRight(struct element ** tree, struct element * node)
{
    struct element * leftChild  = node->left;
    struct element * lcRightSub = node->left->right;

    leftChild->right   = node;
    leftChild->parent  = node->parent;
    node->left         = lcRightSub;
    lcRightSub->parent = node;

    if (node->parent) {
        if (node->parent->left == node) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }
    } else {
        *tree = leftChild;
    }

    node->parent = leftChild;
}

void
insertCase5(struct element ** tree, struct element * node)
{
    struct element *g = grandparent(node);

    node->parent->color = rbBlack;
    g->color            = rbRed;

    if (node == node->parent->left) {
        rotateRight(tree, g);
    } else {
        rotateLeft(tree, g);
    }
}

void
insertCase4(struct element ** tree, struct element * node)
{
    struct element * g = grandparent(node);

    if ((node == node->parent->right) && (node->parent == g->left)) {
        rotateLeft(tree, node->parent);

        /*
         * rotate_left can be the below because of already
         * having *g =  grandparent(n) 
         *
         * struct node *saved_p=g->left, *saved_left_n=n->left;
         * g->left=n; 
         * n->left=saved_p;
         * saved_p->right=saved_left_n;
         * 
         * and modify the parent's nodes properly
         */
        node = node->left; 

    } else if (
            (node == node->parent->left) &&
            (node->parent == g->right)) {

        rotateRight(tree, node->parent);

        /*
         * rotate_right can be the below to take advantage
         * of already having *g =  grandparent(n) 
         *
         * struct node *saved_p=g->right, *saved_right_n=n->right;
         * g->right=n; 
         * n->right=saved_p;
         * saved_p->left=saved_right_n;
         * 
         */
        node = node->right; 
    }

    insertCase5(tree, node);
}

void insertCase1(struct element **, struct element *);

void
insertCase3(struct element ** tree, struct element * node)
{
    struct element * u = uncle(node);
    struct element * g;

    if ((u != NULL) && (u->color == rbRed)) {
        node->parent->color = rbBlack;
        u->color            = rbBlack;
        g                   = grandparent(node);
        g->color            = rbRed;

        insertCase1(tree, g);
    } else {
        insertCase4(tree, node);
    }
}

void
insertCase2(struct element ** tree, struct element * node)
{
    if (node->parent->color == rbBlack) {
        return;
        // Tree is still valid ... wow, again we're done... :)
    } else {
        insertCase3(tree, node);
    }
}

void
insertCase1(struct element ** tree, struct element * node)
{
     if (node->parent == NULL) {
         node->color = rbBlack;
         // we're done.... :)
     } else {
         insertCase2(tree, node);
     }
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    struct element * root1    = NULL;
    struct element * root2    = NULL;
    struct element * inserted = NULL;

    insertElement(&root1, 13);
    insertElement(&root1, 8);
    insertElement(&root1, 16);
    insertElement(&root1, 11);
    insertElement(&root1, 3);
    insertElement(&root1, 9);
    insertElement(&root1, 12);
    insertElement(&root1, 10);

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
    // puts("elements:");
    // for (i=1; i<20; i++) {
    //     struct element * element = findElement(root, i);
    //     printf("Element %02d: n=0x%p p=0x%p l=0x%p r=0x%p\n",
    //             i,
    //             element,
    //             element ? element->parent : 0x0,
    //             element ? element->left : 0x0,
    //             element ? element->right : 0x0);
    // }

    puts("traverse");
    traverse(root1, printElement);

    inserted = insertElement(&root2, 13);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 8);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 16);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 11);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 3);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 9);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 12);
    insertCase1(&root2, inserted);
    inserted = insertElement(&root2, 10);
    insertCase1(&root2, inserted);

    puts("traverse");
    traverse(root2, printElement);

    return 0;
}

// vim: set et ts=4 sw=4:
