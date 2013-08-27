#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVALUES  10

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
    struct element * element = malloc(sizeof(struct element));

    element->data   = data;
    element->color  = rbRed;
    element->parent = NULL;
    element->left   = NULL;
    element->right  = NULL;

    return element;
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
    if (NULL == node) {
        return NULL;
    }

    if (NULL == node->parent->left || node == node->parent->left) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

/*
 * tree modifications...needed for rb handling.
 */
void
rotateLeft(struct element ** tree, struct element * node)
{
    struct element * rightChild = node->right;
    struct element * rcLeftSub  = node->right->left;

    rightChild->left   = node;
    rightChild->parent = node->parent;
    node->right        = rcLeftSub;
    if (NULL != rcLeftSub) {
        rcLeftSub->parent  = node;
    }

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
    if (NULL != lcRightSub) {
        lcRightSub->parent = node;
    }

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
replaceNode(
        struct element ** tree,
        struct element * node1,
        struct element * node2)
{
    if (NULL != node1->parent) {
        if (node1 == node1->parent->left) {
            node1->parent->left = node2;
        } else {
            node1->parent->right = node2;
        }
    } else {
        *tree = node2;
    }

    if (NULL != node2) {
        node2->parent = node1->parent;
    }
}


/**
 * insert element in tree
 */
struct element *
insertElement(struct element ** tree, struct element * element)
{
    struct element * node     = *tree;
    struct element * new_node = NULL;
    struct element * u;
    struct element * g;

    // if tree is empty it's simple... :)
    if (NULL == node) {
        *tree = node = new_node = element;
    } else {
        // normal binary tree add....
        while (element->data != node->data) {
            if (element->data < node->data) {
                if (NULL == node->left) {
                    node->left         = element;
                    node->left->parent = node;
                    new_node = node = node->left;
                    break;
                } else {
                    node = node->left;
                }
            } else if (element->data > node->data) {
                if (NULL == node->right) {
                    node->right         = element;
                    node->right->parent = node;
                    new_node = node = node->right;
                    break;
                } else {
                    node = node->right;
                }
            } else {
                return node;
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

            if (u != NULL && u->color == rbRed) {
                node->parent->color = rbBlack;
                u->color            = rbBlack;
                g->color            = rbRed;

                node = g;
                continue;
            }

            // case 4
            if (node == node->parent->right && node->parent == g->left) {
                rotateLeft(tree, node->parent);
                node = node->left; 
            } else if (node == node->parent->left && node->parent == g->right) {

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
deleteElement(struct element ** tree, struct element * element)
{
    struct element * node = *tree;
    struct element * del_node;
    struct element * child;
    struct element * s;

    // find the relevant node and it's parent
    while (NULL != node && node->data != element->data) {
        if (element->data < node->data) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    // element not found
    if (NULL == node) {
        return node;
    }

    del_node = node;

    // now our cases follows...the first one is the same as with
    // simple binary search trees. Two non null children.

    // case 1: two children
    if (NULL != node->left && NULL != node->right) {
        struct element * successor = findInOrderSuccessor(node);

        node->data = successor->data;
        del_node = node = successor;
    }

    // Precondition: n has at most one non-null child.
    child = (NULL == node->right) ? node->left : node->right;
    replaceNode(tree, node, child);

    // delete one child case
    // TODO this is overly complex as simply derived from the function...
    // maybe this can be simplified. Maybe not...check.
    if (node->color == rbBlack) {
        if (NULL != child && child->color == rbRed) {
            child->color = rbBlack;
            // done despite modifying tree itself if neccessary..
            return del_node;
        } else {
            if (NULL != child) {
                node = child;
            } else {
                node->color = rbBlack;
                node->left  = NULL;
                node->right = NULL;
            }
        }
    } else {
        return del_node;
    }

    // delete and rb rebalance...
    while(1) {
        // case 1
        if (NULL == node->parent) {
            // done again
            break;
        }

        // case 2
        s = sibling(node);

        if (NULL != s && s->color == rbRed) {
            node->parent->color = rbRed;
            s->color            = rbBlack;

            /*
             * detect which child we are...assumption
             * if we are not parent->right and parent->right is not
             * null we must be left, even if its set to NULL previously
             */
            if (NULL != node->parent->right && node != node->parent->right) {
                rotateLeft(tree, node->parent);
            } else {
                rotateRight(tree, node->parent);
            }
        }

        s = sibling(node);
        // case 3 / 4
        if (NULL == s || ((s->color == rbBlack) &&
                    (NULL == s->left || s->left->color == rbBlack) &&
                    (NULL == s->right || s->right->color == rbBlack))) {

            if (NULL != s) {
                s->color = rbRed;
            }

            if (node->parent->color == rbBlack) {
                // case 3
                node = node->parent;
                continue;
            } else {
                // case 4
                node->parent->color = rbBlack;
                // and done again...
                break;
            }
        }

        // case 5
        if  (NULL != s && s->color == rbBlack) {
            // this if statement is trivial,
            // due to case 2 (even though case 2 changed the sibling to a
            // sibling's child,
            // the sibling's child can't be red, since no red parent can
            // have a red child).
            //
            // the following statements just force the red to be on the
            // left of the left of the parent,
            // or right of the right, so case 6 will rotate correctly.
            if ((node == node->parent->left) &&
                    (NULL == s->right || s->right->color == rbBlack) &&
                    (NULL != s->left && s->left->color == rbRed)) {

                // this last test is trivial too due to cases 2-4.
                s->color       = rbRed;
                s->left->color = rbBlack;

                rotateRight(tree, s);
            } else if ((node == node->parent->right) &&
                    (NULL == s->left || s->left->color == rbBlack) &&
                    (NULL != s->right && s->right->color == rbRed)) {
                // this last test is trivial too due to cases 2-4.
                s->color        = rbRed;
                s->right->color = rbBlack;

                rotateLeft(tree, s);
            }
        }

        s = sibling(node);
        // case 6
        if (NULL != s) {
            s->color = node->parent->color;
        }

        if (NULL != node && NULL != node->parent) {
            node->parent->color = rbBlack;

            /*
             * detect which child we are...assumption
             * if we are not parent->right and parent->right is not
             * null we must be left, even if its set to NULL previously
             */
            if (NULL != node->parent->right && node != node->parent->right) {
                if (NULL != s->right) {
                    s->right->color = rbBlack;
                }
                rotateLeft(tree, node->parent);
            } else {
                if (NULL != s->left) {
                    s->left->color = rbBlack;
                }
                rotateRight(tree, node->parent);
            }
        }

        // done...
        break;
    }
 
    //deleteOneChild(tree, node);

    return del_node;
}


void
traverse(struct element * tree, void (*cb)(int, int, enum rbColor))
{
    struct element * previous = tree;
    struct element * node     = tree;
    int              depth    = 1;

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
            cb(node->data, depth, node->color);
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

void printElement(int data, int depth, enum rbColor color)
{
    int  i;

    printf("%s %02d(%02d)", (color==rbRed)?"R":"B", data, depth);
    for (i=0; i<depth; i++) printf("-");
    puts("");
}


/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    struct element * root  = NULL;
    int              value;
    int              count;

//    insertElement(&root, newElement(84));
//    insertElement(&root, newElement(87));
//    insertElement(&root, newElement(78));
//    insertElement(&root, newElement(16));
//    insertElement(&root, newElement(94));
//
//    puts("traverse");
//    traverse(root, printElement);
//
//    free(deleteElement(&root, findElement(root, 87)));
//    puts("traverse");
//    traverse(root, printElement);
//    free(deleteElement(&root, findElement(root, 94)));
//    puts("traverse");
//    traverse(root, printElement);
//    free(deleteElement(&root, findElement(root, 16)));
//    puts("traverse");
//    traverse(root, printElement);
//    free(deleteElement(&root, findElement(root, 84)));
//    puts("traverse");
//    traverse(root, printElement);
//    free(deleteElement(&root, findElement(root, 78)));
//    puts("traverse");
//    traverse(root, printElement);
//
    for (count=0; count<NVALUES;) {
        value = (random() % 1000000) + 1;

        if (NULL == findElement(root, value)) {
            insertElement(&root, newElement(value));
            count++;
        }
    }

    puts("traverse");
    traverse(root, printElement);

    for (count=0; count<NVALUES;) {
        value                    = (random() % 1000000) + 1;
        struct element * element = findElement(root, value);

        if (NULL != element) {
            free(deleteElement(&root, element));
            count++;
        }
    }

    puts("traverse");
    traverse(root, printElement);

    return 0;
}

// vim: set et ts=4 sw=4:
