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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "class.h"
#include "config/value.h"
#include "utils/memory.h"
#include "utils/hash.h"

static
int
configValueCtor(void * _this, va_list * params)
{
	ConfigValue this = _this;

	char   * key    = va_arg(*params, char *);
	size_t   nkey   = va_arg(*params, size_t);
	char   * value  = va_arg(*params, char *);
	size_t   nvalue = va_arg(*params, size_t);

	this->hash = sdbm((unsigned char *)key, nkey);

	/**
	 * if we find the value enclosed by single or double
	 * quotes we take it as a string, else we take it
	 * as a numeric which is treated as a long long
	 * right now.
	 */
	if (('"' == value[0] && '"' == value[nvalue-1])
			|| ('\'' == value[0] && '\'' == value[nvalue-1]))
	{
		this->type = CONFIG_VALUE_STRING;
		(this->value).string = memMalloc(nvalue-1);
		(this->value).string[nvalue-1] = '\0';
		memcpy((this->value).string, value+1, nvalue-2);
		this->nvalue = nvalue;
	} else {
		this->type = CONFIG_VALUE_NUMERIC;
		(this->value).number = atoll(value);
		this->nvalue = sizeof(long long);
	}

	return 0;
}

static
void
configValueDtor(void * _this)
{
	ConfigValue this = _this;

	if (CONFIG_VALUE_STRING == this->type) {
		MEM_FREE((this->value).string);
	}
}

static
unsigned long
configValueGetHash(void * _this)
{
    ConfigValue this = _this;
    
    return this->hash;
}   

static
void
configValueHandleDouble(void * _this, void * _double)
{
	/* right now I do nothing...but I could :D */
}   

INIT_IFACE(Class, configValueCtor, configValueDtor, NULL);
INIT_IFACE(Hashable, configValueGetHash, configValueHandleDouble);
CREATE_CLASS(ConfigValue, NULL, IFACE(Class), IFACE(Hashable));

// vim: set ts=4 sw=4:
