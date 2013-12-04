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

#include "trbase.h"
#include "tree.h"

Tree inOrderSuccessor(Tree);
void treeRotateLeft(Tree *, Tree);
void treeRotateRight(Tree *, Tree);


void *
treeDelete(Tree * this, const void * search, TreeComp comp)
{
	Tree   node = *this;
	Tree   del_node;

	void * data;

	/*
	 * first search for it and if its found return the data
	 * and we are done...
	 */
	while (NULL != node) {
		int comparison = comp(node->data, search);

		if (0 < comparison) {
			node = TREE_LEFT(node);
			continue;
		}

		if (0 > comparison) {
			node = TREE_RIGHT(node);
			continue;
		}

		if (0 == comparison) {
			break;
		}
	}   

	/*
	 * nothing was found...return NULL to indicate this.
	 */
	if (NULL == node) {
		return NULL;
	}

	/*
	 * we found an element, store its data pointer as we are
	 * up to delete it.
	 */
	data = node->data;

	/*
	 * now remove the element.
	 */
	
	/*
	 * if we have two children replace data with the one from 
	 * out inOrderSuccessor and remove the inOrderSuccessor.
	 */
	if (NULL != TREE_LEFT(node) && NULL != TREE_RIGHT(node)) {
		Tree successor = inOrderSuccessor(node);

		node->data = successor->data;
		node       = successor;
	}

	{
		Tree child = TREE_CHILD(node);

		/*
		 * if we still have one child replace ourself with it.
		 */
		TREE_REPLACE_NODE(this, node, child);

		/*
		 * and finally delete the node...and prepare ourselfs
		 * for rebalancing.
		 */
		if (rbBlack == node->color) {
			if (NULL != child && rbRed == child->color) {
				child->color = rbBlack;
				TR_delete(node);
				return data;
			} else {
				del_node = node;
				if (NULL != child) {
					node = child;
				} else {
					node->color = rbBlack;
					node->left  = NULL;
					node->right = NULL;
				}
			}
		} else {
			TR_delete(node);
			return data;
		}
	}

	/*
	 * now comes rebalancing...note that if we came to this point
	 * the node is still not deleted.
	 * This is because I am not sure if it is needed during the
	 * rebalancing process...(this does not make much sense, but
	 * to be honest I don't know now.)
	 */
	while(1) {
		// case 1
		if (NULL == TREE_PARENT(node)) {
			break;
		}

		// case 2
		if (NULL != TREE_SIBLING(node)
				&& rbRed == TREE_SIBLING(node)->color) {

			TREE_PARENT(node)->color  = rbRed;
			TREE_SIBLING(node)->color = rbBlack;

			if (NULL != TREE_PARENT(node)->right &&
					node != TREE_PARENT(node)->right) {

				//TREE_ROTATE_LEFT(this, TREE_PARENT(node));
				treeRotateLeft(this, TREE_PARENT(node));

			} else {

				//TREE_ROTATE_RIGHT(this, TREE_PARENT(node));
				treeRotateRight(this, TREE_PARENT(node));

			}
		}

		// case 3 / 4
		if (NULL == TREE_SIBLING(node)
				|| (rbBlack == TREE_SIBLING(node)->color
					&& (NULL == TREE_SIBLING(node)->left
						|| rbBlack == TREE_SIBLING(node)->left->color)
					&& (NULL == TREE_SIBLING(node)->right
						|| rbBlack == TREE_SIBLING(node)->right->color))) {

			if (NULL != TREE_SIBLING(node)) {
				TREE_SIBLING(node)->color = rbRed;
			}


			/*
			 * this is the point where during the balancing our tree
			 * node can be finally deleted.
			 */
			if (rbBlack == TREE_PARENT(node)->color) {
				// case 3
				Tree parent = node->parent;
				node = parent;
				continue;
			} else {
				// case 4
				TREE_PARENT(node)->color = rbBlack;
				break;
			}
		}

		// case 5
		if (NULL != TREE_SIBLING(node)
				&& rbBlack == TREE_SIBLING(node)->color) {

			if (node == TREE_PARENT(node)->left
					&& (NULL == TREE_SIBLING(node)->right
						|| rbBlack == TREE_SIBLING(node)->right->color)
					&& (NULL != TREE_SIBLING(node)->left
						&& rbRed == TREE_SIBLING(node)->left->color)) {

				TREE_SIBLING(node)->color       = rbRed;
				TREE_SIBLING(node)->left->color = rbBlack;

				//TREE_ROTATE_RIGHT(this, TREE_SIBLING(node));
				treeRotateRight(this, TREE_SIBLING(node));

			} else if (node == TREE_PARENT(node)->right
					&& (NULL == TREE_SIBLING(node)->left
						|| rbBlack == TREE_SIBLING(node)->left->color)
					&& (NULL != TREE_SIBLING(node)->right
						&& rbRed == TREE_SIBLING(node)->right->color)) {

				TREE_SIBLING(node)->color        = rbRed;
				TREE_SIBLING(node)->right->color = rbBlack;

				//TREE_ROTATE_LEFT(this, TREE_SIBLING(node));
				treeRotateLeft(this, TREE_SIBLING(node));
			}
		}
		
		// case 6
		if (NULL != TREE_SIBLING(node)) {
			TREE_SIBLING(node)->color = TREE_PARENT(node)->color;
		}

		if (NULL != node && NULL != TREE_PARENT(node)) {
			TREE_PARENT(node)->color = rbBlack;

			if (NULL != TREE_PARENT(node)->right
					&& node != TREE_PARENT(node)->right) {

				if (NULL != TREE_SIBLING(node)->right) {
					TREE_SIBLING(node)->right->color = rbBlack;
				}
				//TREE_ROTATE_LEFT(this, TREE_PARENT(node));
				treeRotateLeft(this, TREE_PARENT(node));
			} else {
				if (NULL != TREE_SIBLING(node)->left) {
					TREE_SIBLING(node)->left->color = rbBlack;
				}
				//TREE_ROTATE_RIGHT(this, TREE_PARENT(node));
				treeRotateRight(this, TREE_PARENT(node));
			}
		}

		break;
	}

	TR_delete(del_node);
	/*
	 * not sure if deleting here is correct.
	 */
	return data;
}

// vim: set ts=4 sw=4:
