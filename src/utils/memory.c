/**
 * \file This holds all stufff related our memory managent.
 * I try the best as far as I can to reduce memory fragmentation
 * and unneccessary calls to alloc and free.
 *
 * To achive this I try an approach described here as "Quick Fit".
 * http://www.flounder.com/memory_allocation.htm
 *
 * The basic idea is to keep allocated memory segments and don't free
 * them again. Instead I will put them in a tree indexed by their size.
 * To get new memory I first have a look in the tree if there is
 * a fitting memory segment. Fitting mean, larger or exactly the size
 * I need. If there is one, use it. If not create a new one using 
 * usual malloc approach.
 * I won't split the reagions at all because most likely they will be
 * free soon again. This way I might waste some memory, so I have to
 * keep an eye on this.
 *
 * Right now I don't build an upper limit for allocation. The limit
 * still is the system memory itself.
 *
 * This is not implemented as a class because it will be used in the 
 * process of object creation.
 *
 * The data structure is a balanced tree with size as key.
 * Under the size key is a list of elements of the same size.
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2012  Georg Hopp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <search.h>

#include "utils/memory.h"

enum rbColor {rbBlack=1, rbRed=2};

int malloccount = 0;

struct memSegment {
	size_t   size;
	void   * ptr;

	// for address queue
	struct memSegment * next;
	struct memSegment * last;

	// for rbtree
    enum rbColor color;
    struct memSegment * parent;
    struct memSegment * left;
    struct memSegment * right;
};

struct memSegment *
newElement(size_t size)
{
    struct memSegment * element = malloc(size + sizeof(struct memSegment));
	malloccount++;
	printf("+");

    element->size   = size;
    element->ptr    = (void *)element + sizeof(struct memSegment);

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
struct memSegment *
findElement(struct memSegment * tree, size_t size)
{
    struct memSegment * fitting = NULL;

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
struct memSegment *
grandparent(struct memSegment * node)
{
    if (NULL != node && NULL != node->parent) {
        return node->parent->parent;
    }

    return NULL;
}

struct memSegment *
uncle(struct memSegment * node)
{
    struct memSegment * gp = grandparent(node);

    if (NULL == gp) {
        return NULL;
    }

    if (node->parent == gp->left) {
        return gp->right;
    }

    return gp->left;
}

struct memSegment *
sibling(struct memSegment * node)
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
rotateLeft(struct memSegment ** tree, struct memSegment * node)
{
    struct memSegment * rightChild = node->right;
    struct memSegment * rcLeftSub  = node->right->left;

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
rotateRight(struct memSegment ** tree, struct memSegment * node)
{
    struct memSegment * leftChild  = node->left;
    struct memSegment * lcRightSub = node->left->right;

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
        struct memSegment ** tree,
        struct memSegment * node1,
        struct memSegment * node2)
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
struct memSegment *
insertElement(struct memSegment ** tree, struct memSegment * element)
{
    struct memSegment * node     = *tree;
    struct memSegment * new_node = NULL;
    struct memSegment * u;
    struct memSegment * g;

	element->color  = rbRed;
	element->parent = NULL;
	element->left   = NULL;
	element->right  = NULL;
	element->next   = NULL;
	element->last   = NULL;

    // if tree is empty it's simple... :)
    if (NULL == node) {
        *tree = node = new_node = element;
    } else {
        // normal binary tree add....
        while (element->size != node->size) {
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
					node->next = node->last = element;
				} else {
					node->last->next = element;
					node->last       = element;
				}
                return element;
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
struct memSegment *
findInOrderSuccessor(struct memSegment * tree)
{
    struct memSegment * node = tree->right;

    while (NULL != node->left) {
        node = node->left;
    }

    return node;
}

struct memSegment * deleteOneChild(struct memSegment **, struct memSegment *);

struct memSegment *
deleteElement(struct memSegment ** tree, struct memSegment * element)
{
    struct memSegment * node = *tree;
    struct memSegment * del_node;
    struct memSegment * child;
    struct memSegment * s;

    // find the relevant node and it's parent
    while (NULL != node) {
        if (element->size < node->size) {
            node = node->left;
        } else if (element->size > node->size) {
            node = node->right;
        } else {
			// we know we found our tree element.
			if (NULL != node->next) {
				// last element.
				if (NULL != node->parent) {
					if (node->parent->left == node->parent) {
						node->parent->left = node->next;
					} else {
						node->parent->right = node->next;
					}
				}

				if (NULL != node->left) {
					node->left->parent = node->next;
				}

				if (NULL != node->right) {
					node->right->parent = node->next;
				}

				if (node->next == node->last) {
					node->next->next = NULL;
					node->next->last = NULL;
				} else {
					node->next->last = node->last;
				}

				node->next->parent = node->parent;
				node->next->color  = node->color;
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
        struct memSegment * successor = findInOrderSuccessor(node);

		// this is a replacement code for simply change value and remove
		// successor...I can not do this here because the address of
		// the object is part of the information that has to be
		// preserved.
        enum rbColor tmpcolor      = successor->color;
        struct memSegment * tmpparent = successor->parent;
        struct memSegment * tmpleft   = successor->left;
        struct memSegment * tmpright  = successor->right;

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
traverse(struct memSegment * tree, void (*cb)(struct memSegment *, int))
{
    struct memSegment * previous = tree;
    struct memSegment * node     = tree;
    int              depth    = 1;

    /*
     * I think this has something like O(n+log(n)) on a ballanced
     * tree because I have to traverse back the rightmost leaf to
     * the root to get a break condition.
     */
    while (NULL != node) {
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

void printElement(struct memSegment * node, int depth)
{
    int  i;

    printf("%s %010zu:0x%p(%02d)",
			(node->color==rbRed)?"R":"B",
			node->size, 
			node->ptr, 
			depth);

    for (i=0; i<depth; i++) printf("-");
    puts("");
}


struct memSegment * segments = NULL;

// /**
//  * this will interpret any memory segment that is not smaller
//  * than the expected size as fitting.
//  *
//  * @param void * size_ptr  a pointer to a size_t value searched for
//  * @param void * subject   a pointer to the currently analysed tree element
//  */
// static
// int
// segmentFindCmp(const void * size_ptr, const void * subject)
// {
// 	if (*(size_t *)size_ptr > ((struct memSegment *)subject)->size)
// 		return 1;
// 
// 	return 0;
// }
// 
// /**
//  * this returns exact fits....uhh.....I can't relate solely on
//  * the size argument as then same sized segments will never
//  * be stored. 
//  * Maybe a tree is not the best data structure to use to store
//  * these.
//  * Anyway, right now take the ptr into account if size if equal.
//  */
// static
// int
// segmentSearchCmp(const void * search, const void * subject)
// {
// 	size_t idx =
// 		((struct memSegment *)search)->size -
// 		((struct memSegment *)subject)->size;
// 
// 	if (0 == idx) {
// 		return
// 			((struct memSegment *)search)->ptr -
// 			((struct memSegment *)subject)->ptr;
// 	}
// 
// 	return idx;
// }

static
void
segmentFree(struct memSegment * segment, int depth)
{
	while (NULL != segment) {
		struct memSegment * next = segment->next;
		free(segment);
		malloccount--;
		printf("-");
		segment = next;
	}
}


void *
memMalloc(size_t size)
{
	struct memSegment * seg;

	//printf("MALLOC of size: %zu\n", size);
	//traverse(segments, printElement);

	seg = findElement(segments, size);

	if (NULL == seg) {
		seg = newElement(size);
		//printf("  CREATE Segment: 0x%p of size: %zu\n", seg, seg->size);
	} else {
		//printf("  FOUND Segment: 0x%p of size: %zu\n", seg, seg->size);
		// remove the found one from the tree as we use it now.
		deleteElement(&segments, seg);
	}


	return seg->ptr;
}

/**
 * this is a really memory wasting solution....just to be able to
 * use calloc, which might be faster then malloc/memset solution.
 *
 * Maybe this is a bad idea, as we need to memset the buffer anyway
 * if it comes from our tree, which hopefully should be the majority
 * of cases.
 */
void *
memCalloc(size_t nmemb, size_t size)
{
	size_t   _size = nmemb * size;
	void   * mem   = memMalloc(_size);

	memset(mem, 0, _size);

	return mem;
}

void
memFree(void ** mem)
{
	if (NULL != *mem) {
		insertElement(&segments, (struct memSegment *)(*mem - sizeof(struct memSegment)));

		//printf("FREE of Segment: 0x%p of size: %zu\n",
		//		*mem - sizeof(struct memSegment),
		//		((struct memSegment *)(*mem - sizeof(struct memSegment)))->size);
		//traverse(segments, printElement);

		*mem = NULL;
	}
}

void
pre_order(struct memSegment * tree, void (*cb)(struct memSegment *, int))
{
	if (NULL != tree) {
		if (NULL != tree->left) {
			pre_order(tree->left, cb);
		}

		if (NULL != tree->right) {
			pre_order(tree->right, cb);
		}

		cb(tree, 0);
	}
}

void
memCleanup()
{
	printf("\n");
	pre_order(segments, segmentFree);
	printf("\nmalloccount: %d\n", malloccount);
}

void
ffree(void ** data)
{
	if (NULL != *data) {
		free(*data);
		*data = NULL;
	}
}   

// vim: set ts=4 sw=4:
