# Copyright (C) 1991-2012 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

#
#	Sub-makefile for grp portion of the library.
#
include ../option-groups.mak

subdir	:= grp
headers := grp.h

routines := fgetgrent initgroups setgroups \
	    getgrent getgrgid getgrnam putgrent \
	    getgrent_r getgrgid_r getgrnam_r fgetgrent_r

include ../Makeconfig

tests := testgrp
ifneq (y,$(OPTION_EGLIBC_NSSWITCH))
LDLIBS-testgrp += $(shell cat $(common-objpfx)nss/fixed-nsswitch-libs)
endif

ifeq (yes,$(build-shared))
test-srcs :=  tst_fgetgrent
endif


include ../Rules

ifeq ($(have-thread-library),yes)

CFLAGS-getgrgid_r.c = -fexceptions
CFLAGS-getgrnam_r.c = -fexceptions
CFLAGS-getgrent_r.c = -fexceptions
CFLAGS-getgrent.c = -fexceptions
CFLAGS-fgetgrent.c = -fexceptions
CFLAGS-fgetgrent_r.c = -fexceptions $(libio-mtsafe)
CFLAGS-putgrent.c = -fexceptions $(libio-mtsafe)
CFLAGS-initgroups.c = -fexceptions
CFLAGS-getgrgid.c = -fexceptions

endif

ifeq ($(run-built-tests),yes)
# tst_fgetgrent currently only works with shared libraries
ifeq (yes,$(build-shared))
tests: $(objpfx)tst_fgetgrent.out
$(objpfx)tst_fgetgrent.out: tst_fgetgrent.sh $(objpfx)tst_fgetgrent
	$(SHELL) $< $(common-objpfx) '$(run-program-prefix)'

endif
endif
