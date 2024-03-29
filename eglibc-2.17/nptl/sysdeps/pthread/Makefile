# Copyright (C) 2002,2003,2004,2006,2010 Free Software Foundation, Inc.
# This file is part of the GNU C Library.
# Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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

ifeq ($(subdir),nptl)
libpthread-sysdep_routines += errno-loc
ifeq ($(OPTION_EGLIBC_BIG_MACROS),n)
sysdep_routines += small-macros-fns
endif
endif

ifeq ($(subdir),rt)
librt-sysdep_routines += timer_routines librt-cancellation
CFLAGS-librt-cancellation.c += -fexceptions -fasynchronous-unwind-tables

ifeq (yes,$(build-shared))
$(objpfx)tst-timer: $(objpfx)librt.so $(shared-thread-library)
else
$(objpfx)tst-timer: $(objpfx)librt.a $(static-thread-library)
endif

ifeq ($(have-forced-unwind),yes)
tests += tst-mqueue8x
ldflags-libgcc_s = -Wl,--as-needed -lgcc_s -Wl,--no-as-needed
CFLAGS-tst-mqueue8x.c += -fexceptions
LDFLAGS-tst-mqueue8x += $(ldflags-libgcc_s)
endif
endif

ifeq ($(subdir),posix)
CFLAGS-confstr.c += -DLIBPTHREAD_VERSION='"NPTL $(version)"'
endif
