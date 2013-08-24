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

/*
 * function to get specific elements needed for 
 * rb handling, grandparent, uncle and sibbling
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

struct element *
sibling(struct element * node)
{
    return (node == node->parent->left) ?
        node->parent->right :
        node->parent->left;
}

/*
 * rotations...also needed for rb handling.
 */
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

/**
 * insert element in tree
 */
struct element *
insertElement(struct element ** tree, int data)
{
    struct element * node     = *tree;
    struct element * new_node = NULL;
    struct element * u;
    struct element * g;

    // if tree is empty it's simple... :)
    if (NULL == node) {
        *tree = node = new_node = newElement(data);
    } else {
        // normal binary tree add....
        while (data != node->data) {
            if (data < node->data) {
                if (NULL == node->left) {
                    node->left         = newElement(data);
                    node->left->parent = node;
                    new_node = node = node->left;
                    break;
                } else {
                    node = node->left;
                }
            } else {
                if (NULL == node->right) {
                    node->right         = newElement(data);
                    node->right->parent = node;
                    new_node = node = node->right;
                    break;
                } else {
                    node = node->right;
                }
            }
        }
    }

    if (NULL != new_node) {
        /* 
         * handle reballancing rb style
         */
        while (1) {
            // case 1
            if (node->parent == NULL) {
                node->color = rbBlack;
                // we're done.... :)
                break;
            }

            // case 2
            if (node->parent->color == rbBlack) {
                // Tree is still valid ... wow, again we're done... :)
                break;
            }

            // case 3
            u = uncle(node);
            g = grandparent(node);

            if ((u != NULL) && (u->color == rbRed)) {
                node->parent->color = rbBlack;
                u->color            = rbBlack;
                g->color            = rbRed;

                node = g;
                continue;
            }

            // case 4
            if ((node == node->parent->right) && (node->parent == g->left)) {
                rotateLeft(tree, node->parent);
                node = node->left; 
            } else if (
                    (node == node->parent->left) &&
                    (node->parent == g->right)) {

                rotateRight(tree, node->parent);
                node = node->right; 
            }

            // case 5
            g = grandparent(node);

            node->parent->color = rbBlack;
            g->color            = rbRed;

            if (node == node->parent->left) {
                rotateRight(tree, g);
            } else {
                rotateLeft(tree, g);
            }

            // we're done..
            break;
        }
    }

    return new_node;
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

struct element * deleteOneChild(struct element **, struct element *);

struct element *
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
        return node;
    }

    // now our cases follows...the first one is the same as with
    // simple binary search trees.

    // case 1: two children
    if (NULL != node->left && NULL != node->right) {
        struct element * successor = findInOrderSuccessor(node);

        node->data = successor->data;
        node       = successor;
    }

    return deleteOneChild(tree, node);
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


void
replaceNode(struct element * node1, struct element * node2)
{
    if (NULL != node1->parent) {
        if (node1 == node1->parent->left) {
            node1->parent->left = node2;
        } else {
            node1->parent->right = node2;
        }
    }

    if (NULL != node2) {
        node2->parent = node1->parent;
    }
}

void
deleteCase6(struct element ** tree, struct element * node)
{
    struct element * s = sibling(node);

    s->color            = node->parent->color;
    node->parent->color = rbBlack;

    if (node == node->parent->left) {
        s->right->color = rbBlack;
        rotateLeft(tree, node->parent);
    } else {
        s->left->color = rbBlack;
        rotateRight(tree, node->parent);
    }
}

void
deleteCase5(struct element ** tree, struct element * node)
{
    struct element * s = sibling(node);

    if  (NULL != s && s->color == rbBlack) {
        /*
         * this if statement is trivial,
         * due to case 2 (even though case 2 changed the sibling to a
         * sibling's child,
         * the sibling's child can't be red, since no red parent can
         * have a red child).
         */
        /*
         * the following statements just force the red to be on the
         * left of the left of the parent,
         * or right of the right, so case six will rotate correctly.
         */
        if ((node == node->parent->left) &&
                (s->right->color == rbBlack) &&
                (s->left->color == rbRed)) {

            /* this last test is trivial too due to cases 2-4. */
            s->color       = rbRed;
            s->left->color = rbBlack;

            rotateRight(tree, s);
        } else if ((node == node->parent->right) &&
                (s->left->color == rbBlack) &&
                (s->right->color == rbRed)) {
            /*
             * this last test is trivial too due to cases 2-4.
             */
            s->color        = rbRed;
            s->right->color = rbBlack;

            rotateLeft(tree, s);
        }
    }

    deleteCase6(tree, node);
}

void
deleteCase4(struct element ** tree, struct element * node)
{
    struct element * s = sibling(node);

    if ((node->parent->color == rbRed) &&
            (NULL == s || ((s->color == rbBlack) &&
                           (s->left->color == rbBlack) &&
                           (s->right->color == rbBlack)))) {
        if (NULL != s) {
            s->color = rbRed;
        }
        node->parent->color = rbBlack;
    } else {
        deleteCase5(tree, node);
    }
}

void deleteCase1(struct element **, struct element *);

void
deleteCase3(struct element ** tree, struct element * node)
{
    struct element * s = sibling(node);

    if ((node->parent->color == rbBlack) &&
            (NULL == s || ((s->color == rbBlack) &&
                           (s->left->color == rbBlack) &&
                           (s->right->color == rbBlack)))) {
        if (NULL != s) {
            s->color = rbRed;
        }
        deleteCase1(tree, node->parent);
    } else {
        deleteCase4(tree, node);
    }
}

void
deleteCase2(struct element ** tree, struct element * node)
{
    struct element * s = sibling(node);

    if (NULL != s && s->color == rbRed) {
        node->parent->color = rbRed;
        s->color            = rbBlack;

        if (node == node->parent->left) {
            rotateLeft(tree, node->parent);
        } else {
            rotateRight(tree, node->parent);
        }
    }

    deleteCase3(tree, node);
}

void
deleteCase1(struct element ** tree, struct element * node)
{
     if (NULL != node && NULL != node->parent) {
         deleteCase2(tree, node);
     }
}

struct element *
deleteOneChild(struct element ** tree, struct element * node)
{
    /*
     * Precondition: n has at most one non-null child.
     */
    struct element * child = (NULL == node->right) ? node->left : node->right;

    replaceNode(node, child);

    if (node->color == rbBlack) {
        if (NULL != child && child->color == rbRed) {
            child->color = rbBlack;
        } else {
            deleteCase1(tree, child);
        }
    }

    if (NULL == node->parent){
        *tree = 0x0;
    }

    return node;
}



/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    struct element * root     = NULL;

    insertElement(&root, 13);
    insertElement(&root, 8);
    insertElement(&root, 16);
    insertElement(&root, 11);
    insertElement(&root, 3);
    insertElement(&root, 9);
    insertElement(&root, 12);
    insertElement(&root, 10);
    
    puts("traverse");
    traverse(root, printElement);

    /*
    free(deleteElement(&root, 8));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 11));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 13));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 3));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 16));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 10));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 9));
    puts("traverse");
    traverse(root, printElement);

    free(deleteElement(&root, 12));
    puts("traverse");
    traverse(root, printElement);
    */

    return 0;
}

// vim: set et ts=4 sw=4:
