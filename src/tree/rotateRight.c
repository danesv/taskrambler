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

void
treeRotateRight(Tree * this, Tree node)
{
	Tree leftChild  = TREE_LEFT(node);
	Tree lcRightSub = TREE_LEFT_RIGHT(node);

	leftChild->right   = node;
	leftChild->parent  = TREE_PARENT(node);
	node->left         = lcRightSub;
	if (NULL != lcRightSub) {
		lcRightSub->parent = node;
	}   

	if (NULL != TREE_PARENT(node)) {
		if (TREE_PARENT(node)->left == node) {
			TREE_PARENT(node)->left = leftChild;
		} else {
			TREE_PARENT(node)->right = leftChild;
		}   
	} else {
		*this = leftChild;
	}   

	node->parent = leftChild;
}

// vim: set ts=4 sw=4:
