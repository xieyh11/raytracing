/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <assert.h>
#include "pthreadP.h"


int
__pthread_attr_getstack (attr, stackaddr, stacksize)
     const pthread_attr_t *attr;
     void **stackaddr;
     size_t *stacksize;
{
  struct pthread_attr *iattr;

  assert (sizeof (*attr) >= sizeof (struct pthread_attr));
  iattr = (struct pthread_attr *) attr;

  /* Store the result.  */
#ifdef _STACK_GROWS_DOWN
  *stackaddr = (char *) iattr->stackaddr - iattr->stacksize;
#else
  *stackaddr = (char *) iattr->stackaddr;
#endif
  *stacksize = iattr->stacksize;

  return 0;
}
strong_alias (__pthread_attr_getstack, pthread_attr_getstack)
