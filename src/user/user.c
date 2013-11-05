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

#include "user.h"
#include "uuid.h"
#include "class.h"

#include "interface/serializable.h"
#include "interface/indexable.h"

#include "utils/memory.h"


static
int
userCtor(void * _this, va_list * params)
{
	User   this  = _this;
	char * email = va_arg(* params, char *);

	if (NULL != email) {
		size_t   nemail     = va_arg(* params, size_t);
		char   * firstname  = va_arg(* params, char *);
		size_t   nfirstname = va_arg(* params, size_t);
		char   * surname    = va_arg(* params, char *);
		size_t   nsurname   = va_arg(* params, size_t);

		size_t storage_size = 
			nemail + 1 +
			nfirstname + 1 +
			nsurname + 1 +
			3 * sizeof(size_t);

		this->email = memMalloc(storage_size);
		memcpy(this->email, email, nemail);
		this->email[nemail] = '\0';

		this->firstname = this->email + nemail + 1;
		memcpy(this->firstname, firstname, nfirstname);
		this->firstname[nfirstname] = '\0';

		this->surname = this->firstname + nfirstname + 1;
		memcpy(this->surname, surname, nsurname);
		this->surname[nsurname] = '\0';

		this->nemail  = (size_t *)(this->surname + nsurname + 1);
		*this->nemail = nemail;

		this->nfirstname  = this->nemail + 1;
		*this->nfirstname = nfirstname;

		this->nsurname  = this->nfirstname + 1;
		*this->nsurname = nsurname;
	}

	return 0;
}

static
void
userDtor(void * _this)
{
	User this = _this;

	if (NULL != this->email) {
		MEM_FREE(this->email);
	}
}

static
void
userSerialize(
		void           * _this,
		unsigned char ** serialized,
		size_t         * nserialized)
{
	User this = _this;

	*nserialized =
		*this->nemail + 1 +
		*this->nfirstname + 1 +
		*this->nsurname + 1 + 
		3 * sizeof(size_t); 

	*serialized = memMalloc(*nserialized);

	memcpy(*serialized, this->email, *nserialized);
}

static
void
userUnserialize(
		void                * _this,
		const unsigned char * serialized,
		size_t                nserialized)
{
	User     this = _this;
	size_t * user_data_sizes;

	this->email = memMalloc(nserialized);
	memcpy(this->email, serialized, nserialized);

	user_data_sizes =
		(size_t *)(this->email + nserialized - 3 * sizeof(size_t));

	this->nemail     = user_data_sizes;
	this->nfirstname = user_data_sizes + 1;
	this->nsurname   = user_data_sizes + 2;

	this->firstname = this->email + *this->nemail + 1;
	this->surname   = this->firstname + *this->nfirstname + 1;
}

static
Uuid
userIndexUuid(void * _this, Uuid namespace)
{
	User this = _this;

	return uuidVersion3(
			(unsigned char *)this->email,
			*this->nemail,
			namespace);
}


INIT_IFACE(Class, userCtor, userDtor, NULL);
INIT_IFACE(Serializable, userSerialize, userUnserialize);
INIT_IFACE(Indexable, userIndexUuid);
CREATE_CLASS(User, NULL, IFACE(Class), IFACE(Serializable), IFACE(Indexable));

// vim: set ts=4 sw=4:
