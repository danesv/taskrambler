/**
 * \file
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

#ifndef __TREE_H__
#define __TREE_H__

#include "class.h"

#define TREE_RIGHT(node)	(NULL!=(node)?(node)->right:NULL)
#define TREE_LEFT(node)		(NULL!=(node)?(node)->left:NULL)
#define TREE_PARENT(node)	(NULL!=(node)?(node)->parent:NULL)

#define TREE_CHILD(node) \
	(NULL==TREE_RIGHT((node))?TREE_LEFT((node)):TREE_RIGHT((node)))

#define TREE_RIGHT_LEFT(node) \
	(NULL!=TREE_RIGHT((node))?TREE_LEFT(TREE_RIGHT((node))):NULL)

#define TREE_LEFT_RIGHT(node) \
	(NULL!=TREE_LEFT((node))?TREE_RIGHT(TREE_LEFT((node))):NULL)

#define TREE_SIBLING(node)               \
	(NULL!=TREE_PARENT((node))?          \
	 ((node)==TREE_PARENT((node))->left? \
	  TREE_PARENT((node))->right:        \
	  TREE_PARENT((node))->left):        \
	 NULL)

#define TREE_GRANDPARENT(node) \
	(NULL!=TREE_PARENT((node))?TREE_PARENT((node))->parent:NULL)

#define TREE_UNCLE(node)                                   \
	(NULL!=TREE_GRANDPARENT((node))?                       \
	 (TREE_PARENT((node))==TREE_GRANDPARENT((node))->left? \
	  TREE_GRANDPARENT((node))->right:                     \
	  TREE_GRANDPARENT((node))->left):                     \
	 NULL)

#define TREE_ROTATE_LEFT(root, node)                        \
	do {                                                    \
		if (NULL != TREE_RIGHT_LEFT((node))) {              \
			TREE_RIGHT_LEFT((node))->parent = (node);       \
		}                                                   \
		TREE_RIGHT((node))->left = (node);                  \
		if (NULL != TREE_PARENT((node))) {                  \
			if (TREE_PARENT((node))->left==(node)) {        \
				TREE_PARENT((node))->left = (node)->right;  \
			} else {                                        \
				TREE_PARENT((node))->right = (node)->right; \
			}                                               \
		} else {                                            \
			*(root) = (node)->right;                        \
		}                                                   \
		(node)->right  = TREE_RIGHT_LEFT((node));           \
		(node)->parent = (node)->right;                     \
		TREE_RIGHT((node))->parent = (node)->parent;        \
	} while(0)

#define TREE_ROTATE_RIGHT(root, node)                       \
	do {                                                    \
		if (NULL != TREE_LEFT_RIGHT((node))) {              \
			TREE_LEFT_RIGHT((node))->parent = (node);       \
		}                                                   \
		TREE_LEFT((node))->right = (node);                  \
		if (NULL != TREE_PARENT((node))) {                  \
			if (TREE_PARENT((node))->left==(node)) {        \
				TREE_PARENT((node))->left = (node)->left;   \
			} else {                                        \
				TREE_PARENT((node))->right = (node)->left;  \
			}                                               \
		} else {                                            \
			*(root) = (node)->left;                         \
		}                                                   \
		TREE_LEFT((node))->parent = (node)->parent;         \
		(node)->left   = TREE_LEFT_RIGHT((node));           \
		(node)->parent = (node)->right;                     \
	} while(0)

#define TREE_REPLACE_NODE(root, node1, node2)            \
	do {                                                 \
		if (NULL != TREE_PARENT((node1))) {              \
			if ((node1) == TREE_PARENT((node1))->left) { \
				TREE_PARENT((node1))->left = (node2);    \
			} else {                                     \
				TREE_PARENT((node1))->right = (node2);   \
			}                                            \
		} else {                                         \
			*(root) = (node2);                           \
		}                                                \
		if (NULL != (node2)) {                           \
			(node2)->parent = (node1)->parent;           \
		}                                                \
	} while(0)


enum rbColor {rbBlack=1, rbRed=2};

CLASS(Tree) {
	void * data;

	enum rbColor color;

	Tree parent;
	Tree left;
	Tree right;
};

typedef int  (*TreeComp)(const void *, const void *);
typedef void (*TreeAction)(const void *, const int);

void * treeFind(Tree, const void *, TreeComp);
void * treeInsert(Tree *, const void *, TreeComp);
void * treeDelete(Tree *, const void *, TreeComp);
void   treeWalk(Tree, TreeAction);
void   treeDestroy(Tree *, TreeAction);

#endif // __TREE_H__

// vim: set ts=4 sw=4:
