# Copyright (C) 1993, 94, 95, 96, 97, 99 Free Software Foundation, Inc.
# This file is part of the GNU C Library.
# Contributed by Brendan Kehoe (brendan@zen.org).

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library.  If not, see
# <http://www.gnu.org/licenses/>.

ifeq ($(subdir),db2)
CPPFLAGS += -DHAVE_SPINLOCKS=1 -DHAVE_ASSEM_ALPHA=1
endif

ifeq ($(subdir),gmon)
sysdep_routines += _mcount
endif

ifeq ($(subdir),gnulib)
sysdep_routines += divl divlu divq divqu reml remlu remq remqu
endif

ifeq ($(subdir),string)
sysdep_routines += stxcpy stxncpy
endif

ifeq ($(subdir),elf)
# The ld.so startup code cannot use literals until it self-relocates.
# It uses more than 64k for the small data area.
CFLAGS-rtld.c = -mbuild-constants $(PIC-ccflag)
endif

ifeq ($(subdir),math)
# The fma routines rely on inexact being raised for correct results.
CFLAGS-s_fma.c = -mieee-with-inexact
CFLAGS-s_fmaf.c = -mieee-with-inexact
# This test tries to check for inexact being raised by arithmetic.
CFLAGS-test-misc.c += -mieee-with-inexact
# Avoid "conflicting types for built-in function" warnings
CFLAGS-s_isnan.c += -fno-builtin-isnanf
endif

# Build everything with full IEEE math support, and with dynamic rounding;
# there are a number of math routines that are defined to work with the
# "current" rounding mode, and it's easiest to set this with all of them.
sysdep-CFLAGS += -mieee -mfp-rounding-mode=d

# libc.so requires about 16k for the small data area, which is well
# below the 64k maximum.
pic-ccflag = -fpic
