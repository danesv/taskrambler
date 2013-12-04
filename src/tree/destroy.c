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

void
treeDestroy(Tree * this, TreeAction action)
{
    Tree previous = * this;
    Tree node     = * this;
    int  depth    = 1;
    
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
        if (((NULL == TREE_LEFT(node)
				|| previous == TREE_LEFT(node)) && NULL == TREE_RIGHT(node))
				|| previous == TREE_RIGHT(node)) {
            
            Tree parent = TREE_PARENT(node);
            
            action(node->data, depth);
            
            previous = node;
			TR_delete(node);
            node = parent;
            depth--;

            continue;
        }
        
        if ((NULL == TREE_LEFT(node) || previous == TREE_LEFT(node))) {
            /*
             * If there are no more elements to the left or we
             * came from the left, process data.
             */
            previous = node;
            
            if (NULL != TREE_RIGHT(node)) {
                node = TREE_RIGHT(node);
                depth++;
            } else { 
                node = TREE_PARENT(node);
                depth--;
            }
        } else {
            /*
             * if there are more elements to the left go there.
             */
            previous = node;
            node     = TREE_LEFT(node);
            depth++;
        }
    }
	
	*this = NULL;
}

// vim: set ts=4 sw=4:
