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

#include "tree.h"

void treeRotateLeft(Tree *, Tree);
void treeRotateRight(Tree *, Tree);

void *
treeInsert(Tree * this, const void * search, TreeComp comp)
{
	Tree node     = *this;
	Tree new_node = NULL;

	/*
	 * insert the node or return the one in tree if comparison
	 * succeeds.
	 */
	if (NULL == node) {
		/*
		 * if the root is NULL we simple add the element and set
		 * node to it.
		 */
		*this = node = new_node = new(Tree, search);
	} else {
		/*
		 * first search for it and if its found return the data
		 * and we are done...
		 */
		int  comparison;

		while (NULL != node) {
			comparison = comp(node->data, search);

			if (0 < comparison) {
				if (NULL != TREE_LEFT(node)) {
					node = TREE_LEFT(node);
					continue;
				} else {
					break;
				}
			}

			if (0 > comparison) {
				if (NULL != TREE_RIGHT(node)) {
					node = TREE_RIGHT(node);
					continue;
				} else {
					break;
				}
			}

			if (0 == comparison) {
				return node->data;
			}
		}   

		/*
		 * as we have not found it now add a new element.
		 */
		if (0 < comparison) {
			node->left = new(Tree, search);
			TREE_LEFT(node)->parent = node;
			node = new_node = TREE_LEFT(node);
		} else {
			node->right = new(Tree, search);
			TREE_RIGHT(node)->parent = node;
			node = new_node = TREE_RIGHT(node);
		}
	}

	/*
	 * we expect node not to be NULL and pointing to our
	 * new node at this point...now rabalance the tree
	 */
	while (1) {
		// case 1
		if (NULL == TREE_PARENT(node)) {
			node->color = rbBlack;
			break;
		}

		// case 2
		if (rbBlack == TREE_PARENT(node)->color) {
			break;
		}

		// case 3
		if (NULL != TREE_UNCLE(node) && rbRed == TREE_UNCLE(node)->color) {
			TREE_PARENT(node)->color      = rbBlack;
			TREE_UNCLE(node)->color       = rbBlack;
			TREE_GRANDPARENT(node)->color = rbRed;

			node = TREE_GRANDPARENT(node);
			continue;
		}

		// case 4
		if (node == TREE_PARENT(node)->right
				&& TREE_PARENT(node) == TREE_GRANDPARENT(node)->left) {

			//TREE_ROTATE_LEFT(this, TREE_PARENT(node));
			treeRotateLeft(this, TREE_PARENT(node));
			node = TREE_LEFT(node);

		} else if (node == TREE_PARENT(node)->left
				&& TREE_PARENT(node) == TREE_GRANDPARENT(node)->right) {

			//TREE_ROTATE_RIGHT(this, TREE_PARENT(node));
			treeRotateRight(this, TREE_PARENT(node));
			node = TREE_RIGHT(node);

		}

		// case 5
		TREE_PARENT(node)->color      = rbBlack;
		TREE_GRANDPARENT(node)->color = rbRed;

		if (node == TREE_PARENT(node)->left) {
			//TREE_ROTATE_RIGHT(this, TREE_GRANDPARENT(node));
			treeRotateRight(this, TREE_GRANDPARENT(node));
		} else {
			//TREE_ROTATE_LEFT(this, TREE_GRANDPARENT(node));
			treeRotateLeft(this, TREE_GRANDPARENT(node));
		}

		break;
	}

	return new_node->data;
}

// vim: set ts=4 sw=4:
