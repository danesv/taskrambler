/**
 * \file
 *
 * \author	Georg Hopp
 *
 * \copyright
 * Copyright © 2014 Georg Hopp
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
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>

#include "trbase.h"
#include "trdata.h"

#include "config/config.h"
#include "config/value.h"

static
int
configCtor(void * _this, va_list * params)
{
	Config this = _this;

	FILE   * handle;
	char     line[MAX_CONFIG_LINE];
	char   * cnf_file  = va_arg(*params, char *);
	size_t   ncnf_file = strlen(cnf_file);

	this->cnf_file = TR_malloc(ncnf_file + 1);
	memcpy(this->cnf_file, cnf_file, ncnf_file);
	this->cnf_file[ncnf_file] = '\0';

	handle = fopen(this->cnf_file, "r");
	if (NULL == handle) {
		TR_MEM_FREE(this->cnf_file);
		return -1;
	}

	this->config = TR_new(TR_Hash);

	line[MAX_CONFIG_LINE] = '\0';

	while(NULL != fgets(line, MAX_CONFIG_LINE, handle)) {
		char   * key     = line;
		size_t   nkey    = 0;
		size_t   nvalue  = 0;
		size_t   nspaces = 0;
		char   * value;

		while (isspace(*key)) {
			key++;
		}

		if ('#' == *key || 0 == *key) {
			continue;
		}

		while (! isspace(key[nkey])) {
			nkey++;
		}

		value = &(key[nkey+1]);
		while (isspace(*value)) {
			value++;
		}

		while ('\0' != value[nvalue+nspaces]
				&& '\n' != value[nvalue+nspaces])
		{
			if (isspace(value[nvalue+nspaces])) {
				nspaces++;
			} else {
				if (0 != nspaces) {
					nvalue += nspaces;
					nspaces = 0;
				}
				nvalue++;
			}
		}

		value[nvalue] = '\0';

		if (0 != nkey && 0 != nvalue) {
			TR_hashAdd(
					this->config,
					TR_new(ConfigValue, key, nkey, value, nvalue));
		}
	}

	fclose(handle);

	return 0;
}

static
void configDtor(void * _this)
{
	Config this = _this;

	TR_MEM_FREE(this->cnf_file);
	TR_delete(this->config);
}

TR_INIT_IFACE(TR_Class, configCtor, configDtor, NULL);
TR_CREATE_CLASS(Config, NULL, NULL, TR_IF(TR_Class));

// vim: set ts=4 sw=4:
