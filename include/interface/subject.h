/**
 * \file
 * Definition of the subject pattern implementation.
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

#ifndef __SUBJECT_H__
#define __SUBJECT_H__

typedef void (* fptr_subjectAttach)(void *, void *);
typedef void (* fptr_subjectDetach)(void *, void *);
typedef void (* fptr_subjectNotify)(void *);

extern const struct interface i_Subject;

struct i_Subject {
	const struct interface * const _;
	fptr_subjectAttach       attach;
	fptr_subjectDetach       detach;
	fptr_subjectNotify       notify;
};

extern void subjectAttach(void *, void *);
extern void subjectDetach(void *, void *);
extern void subjectNotify(void *);

#endif // __SUBJECT_H__

// vim: set ts=4 sw=4:
