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
#include "trbase.h"


static
int
userCtor(void * _this, va_list * params)
{
	User   this     = _this;
	char * username = va_arg(* params, char *);

	if (NULL != username) {
		size_t   nusername  = va_arg(* params, size_t);
		char   * email      = va_arg(* params, char *);
		size_t   nemail     = va_arg(* params, size_t);
		char   * firstname  = va_arg(* params, char *);
		size_t   nfirstname = va_arg(* params, size_t);
		char   * surname    = va_arg(* params, char *);
		size_t   nsurname   = va_arg(* params, size_t);

		size_t storage_size = 
			nusername + 1 +
			nemail + 1 +
			nfirstname + 1 +
			nsurname + 1 +
			4 * sizeof(size_t);

		this->username = TR_malloc(storage_size);
		memcpy(this->username, username, nusername);
		this->username[nusername] = '\0';

		this->email = this->username + nusername + 1;
		memcpy(this->email, email, nemail);
		this->email[nemail] = '\0';

		this->firstname = this->email + nemail + 1;
		memcpy(this->firstname, firstname, nfirstname);
		this->firstname[nfirstname] = '\0';

		this->surname = this->firstname + nfirstname + 1;
		memcpy(this->surname, surname, nsurname);
		this->surname[nsurname] = '\0';

		this->nusername  = (size_t *)(this->surname + nsurname + 1);
		*this->nusername = nusername;

		this->nemail  = this->nusername + 1;
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

	if (NULL != this->username) {
		TR_MEM_FREE(this->username);
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
		*this->nusername + 1 +
		*this->nemail + 1 +
		*this->nfirstname + 1 +
		*this->nsurname + 1 + 
		4 * sizeof(size_t); 

	*serialized = TR_malloc(*nserialized);

	memcpy(*serialized, this->username, *nserialized);
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

	this->username = TR_malloc(nserialized);
	memcpy(this->username, serialized, nserialized);

	user_data_sizes =
		(size_t *)(this->username + nserialized - 4 * sizeof(size_t));

	this->nusername  = user_data_sizes;
	this->nemail     = user_data_sizes + 1;
	this->nfirstname = user_data_sizes + 2;
	this->nsurname   = user_data_sizes + 3;

	this->email     = this->username + *this->nusername + 1;
	this->firstname = this->email + *this->nemail + 1;
	this->surname   = this->firstname + *this->nfirstname + 1;
}

static
void *
userIndexUuid(void * _this, void * _namespace)
{
	User this      = _this;
	Uuid namespace = _namespace;

	return uuidVersion3(
			(unsigned char *)this->username,
			*this->nusername,
			namespace);
}


TR_INIT_IFACE(TR_Class, userCtor, userDtor, NULL);
TR_INIT_IFACE(TR_Serializable, userSerialize, userUnserialize);
TR_INIT_IFACE(TR_Indexable, userIndexUuid);
TR_CREATE_CLASS(
		User,
		NULL,
		TR_IF(TR_Class),
		TR_IF(TR_Serializable),
		TR_IF(TR_Indexable));

// vim: set ts=4 sw=4:
