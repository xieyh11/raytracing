/* Copyright (C) 2002-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#if defined (_MIPS_SZPTR)
# define __WORDSIZE	_MIPS_SZPTR
#elif defined (__INTPTR_WIDTH__)
# define __WORDSIZE	__INTPTR_WIDTH__
#else
# error Cannot define __WORDSIZE
#endif
#if _MIPS_SIM == _ABI64
# define __WORDSIZE_TIME64_COMPAT32	1
#endif
