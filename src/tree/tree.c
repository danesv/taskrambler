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

#define _GNU_SOURCE

#include <stdarg.h>

#include "tree.h"
#include "class.h"

static
int
treeCtor(void * _this, va_list * params)
{
	Tree this = _this;

	this->data   = va_arg(*params, void *);
	this->color  = rbRed;
	this->parent = NULL;
	this->left   = NULL;
	this->right  = NULL;

	return 0;
}

static
void
treeDtor(void * _this)
{
}

INIT_IFACE(Class, treeCtor, treeDtor, NULL);
CREATE_CLASS(Tree, NULL, IFACE(Class));

// vim: set ts=4 sw=4:
