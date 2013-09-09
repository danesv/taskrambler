#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVALUES  10

enum rbColor {rbBlack=1, rbRed=2};


struct element
{
    size_t   size;
    void   * ptr;

    enum rbColor color;

    struct element * next;
    struct element * last;

    struct element * parent;
    struct element * left;
    struct element * right;
};

struct element *
newElement(size_t size)
{
    struct element * element = malloc(size + sizeof(struct element));

    element->size   = size;
    element->ptr    = element + sizeof(struct element);

    element->next   = NULL;
    element->last   = NULL;

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
findElement(struct element * tree, size_t size)
{
    struct element * fitting = NULL;

    while (NULL != tree) {
        if (tree->size == size) {
            fitting = tree;
            break;
        }

        if (size > tree->size) {
            tree = tree->right;
        } else {
            fitting = tree;
            tree    = tree->left;
        }
    }

    return fitting;
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

    element->next   = NULL;
    element->last   = NULL;

    element->color  = rbRed;
    element->parent = NULL;
    element->left   = NULL;
    element->right  = NULL;

    // if tree is empty it's simple... :)
    if (NULL == node) {
        *tree = node = new_node = element;
    } else {
        // normal binary tree add....
        while (NULL != node) {
            if (element->size < node->size) {
                if (NULL == node->left) {
                    node->left         = element;
                    node->left->parent = node;
                    new_node = node = node->left;
                    break;
                } else {
                    node = node->left;
                }
            } else if (element->size > node->size) {
                if (NULL == node->right) {
                    node->right         = element;
                    node->right->parent = node;
                    new_node = node = node->right;
                    break;
                } else {
                    node = node->right;
                }
            } else {
                if (NULL == node->next) {
                    node->next = element;
                    node->last = element;
                } else {
                    node->last->next = element;
                    node->last       = element;
                }
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
 *
 * 2: *successor = {size = 80, ptr = 0x603ae0, color = rbRed, parent = 0x603160, 
 *   left = 0x0, right = 0x0}
 *   1: *node = {size = 70, ptr = 0x603a60, color = rbBlack, parent = 0x603070, 
 *     left = 0x6030e0, right = 0x6031e0}
 *
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
    while (NULL != node) {

        if (element->size < node->size) {
            node = node->left;
        } else if (element->size > node->size) {
            node = node->right;
        } else {
            if (NULL != node->next) {
                if (NULL != node->parent) {
                    if (node == node->parent->left) {
                        node->parent->left = node->next;
                    } else {
                        node->parent->right = node->next;
                    }
                } else {
                    *tree = node->next;
                }

                if (NULL != node->left) {
                    node->left->parent = node->next;
                }

                if (NULL != node->right) {
                    node->right->parent = node->next;
                }
                
                node->next->last   = node->last;
                node->next->color  = node->color;
                node->next->parent = node->parent;
                node->next->left   = node->left;
                node->next->right  = node->right;

                return node;
            }
            break;
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

        enum rbColor tmpcolor      = successor->color;
        struct element * tmpparent = successor->parent;
        struct element * tmpleft   = successor->left;
        struct element * tmpright  = successor->right;

        replaceNode(tree, node, successor);

        successor->color        = node->color;
        successor->left         = node->left;
        successor->left->parent = successor;
        // the right one might be successor...
        if (node->right == successor) {
            successor->right = node;
            node->parent     = successor;
        } else {
            successor->right    = node->right;
            node->right->parent = successor;
            node->parent        = tmpparent;
            tmpparent->left     = node;
        }

        node->color  = tmpcolor;
        node->left   = tmpleft;
        node->right  = tmpright;
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
traverse(struct element * tree, void (*cb)(struct element *, int))
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
            cb(node, depth);
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

void
post(struct element * tree, void (*cb)(struct element *, int))
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
        if ((NULL == node->left && NULL == node->right)
                || previous == node->right) {

            struct element * parent = node->parent;

            cb(node, depth);

            previous = node;
            node     = parent;
            depth--;
            continue;
        }

        if ((NULL == node->left || previous == node->left)) {
            /*
             * If there are no more elements to the left or we
             * came from the left, process data.
             */
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

void printElement(struct element * node, int depth)
{
    int  i;

    printf("%s %010zu:%p(%02d)",
            (node->color==rbRed)?"R":"B",
            node->size,
            node->ptr,
            depth);
    for (i=0; i<depth; i++) printf("-");
    puts("");

    node = node->next;
    while (NULL != node) {
        printf("  %s %010zu:%p(%02d)",
                (node->color==rbRed)?"R":"B",
                node->size,
                node->ptr,
                depth);
        for (i=0; i<depth; i++) printf("-");
        puts("");
        node = node->next;
    }
}

void cleanup(struct element * node, int depth)
{
    while (NULL != node) {
        printf("free node: ");
        printElement(node, 0);
        struct element * next = node->next;
        free(node);
        node = next;
    }
}

/**
 * =======================================================================
 */
int
main(int argc, char * argv[])
{
    struct element * root  = NULL;
    struct element * found = NULL;

    insertElement(&root, newElement(40));
    insertElement(&root, newElement(50));
    insertElement(&root, newElement(60));
    insertElement(&root, newElement(70));
    insertElement(&root, newElement(80));
    insertElement(&root, newElement(45));
    insertElement(&root, newElement(75));
    insertElement(&root, newElement(85));
    puts("traverse");
    traverse(root, printElement);
    puts("");

    insertElement(&root, newElement(70));
    puts("traverse");
    traverse(root, printElement);
    puts("");

    found = findElement(root, 10);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 10);
    } else {
        printElement(found, 0);
    }
    puts("");

    found = findElement(root, 64);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 64);
    } else {
        printElement(found, 0);
    }
    puts("");

    found = findElement(root, 90);
    if (NULL == found) {
        printf("can't find segmenet of minimum size: %d\n", 90);
    } else {
        printElement(found, 0);
    }
    puts("");

    free(deleteElement(&root, findElement(root, 70)));
    puts("traverse");
    traverse(root, printElement);
    puts("");

    insertElement(&root, newElement(80));
    insertElement(&root, newElement(50));
    insertElement(&root, newElement(80));

    puts("traverse");
    traverse(root, printElement);
    puts("");

    found = deleteElement(&root, findElement(root, 80));
    printf("up to free: %p\n", found);
    free(found);
    puts("traverse");
    traverse(root, printElement);
    puts("");

    found = deleteElement(&root, findElement(root, 50));
    printf("up to free: %p\n", found);
    free(found);
    puts("traverse");
    traverse(root, printElement);
    puts("");

    found = deleteElement(&root, findElement(root, 70));
    printf("up to free: %p\n", found);
    free(found);
    puts("traverse");
    traverse(root, printElement);
    puts("");

//    post(root, cleanup);
//
    return 0;
}

// vim: set et ts=4 sw=4:
