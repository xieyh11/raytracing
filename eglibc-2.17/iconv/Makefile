# Copyright (C) 1997,1998,2000-2004,2007,2009,2012
# Free Software Foundation, Inc.
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
#	Makefile for iconv.
#
include ../option-groups.mak

subdir	:= iconv

include ../Makeconfig

headers		= iconv.h gconv.h
routines	= iconv_open iconv iconv_close \
		  gconv_open gconv gconv_close gconv_db gconv_conf \
		  gconv_builtin gconv_simple gconv_trans gconv_cache
routines	+= gconv_dl

vpath %.c ../locale/programs ../intl

iconv_prog-modules = iconv_charmap charmap charmap-dir linereader \
		     dummy-repertoire simple-hash xstrdup xmalloc
iconvconfig-modules = strtab xmalloc hash-string
extra-objs	   = $(iconv_prog-modules:=.o) $(iconvconfig-modules:=.o)
CFLAGS-iconv_prog.c = -I../locale/programs
CFLAGS-iconv_charmap.c = -I../locale/programs
CFLAGS-dummy-repertoire.c = -I../locale/programs
CFLAGS-charmap.c = -DCHARMAP_PATH='"$(i18ndir)/charmaps"' \
		   -DDEFAULT_CHARMAP=null_pointer -DNEED_NULL_POINTER
CFLAGS-linereader.c = -DNO_TRANSLITERATION
CFLAGS-simple-hash.c = -I../locale

tests	= tst-iconv1 tst-iconv2 tst-iconv3 tst-iconv4 tst-iconv5

others		= iconv_prog iconvconfig
install-others-programs	= $(inst_bindir)/iconv
install-sbin	= iconvconfig

CFLAGS-gconv_cache.c += -DGCONV_DIR='"$(gconvdir)"'
CFLAGS-gconv_conf.c = -DGCONV_PATH='"$(gconvdir)"'
CFLAGS-iconvconfig.c = -DGCONV_PATH='"$(gconvdir)"' -DGCONV_DIR='"$(gconvdir)"'

CPPFLAGS-iconv_prog = -DNOT_IN_libc
CPPFLAGS-iconv_charmap = -DNOT_IN_libc
CPPFLAGS-iconvconfig = -DNOT_IN_libc
CPPFLAGS-linereader = -DNOT_IN_libc
CPPFLAGS-strtab = -DNOT_IN_libc
CPPFLAGS-charmap = -DNOT_IN_libc
CPPFLAGS-charmap-dir = -DNOT_IN_libc
ifneq (y,$(OPTION_EGLIBC_SPAWN))
CPPFLAGS-charmap-dir.c += -DNO_UNCOMPRESS
endif

include ../Rules

$(inst_bindir)/iconv: $(objpfx)iconv_prog $(+force)
	$(do-install-program)

$(objpfx)iconv_prog: $(iconv_prog-modules:%=$(objpfx)%.o)
$(objpfx)iconvconfig: $(iconvconfig-modules:%=$(objpfx)%.o)

ifeq ($(run-built-tests),yes)
xtests: test-iconvconfig
endif

.PHONY: test-iconvconfig
test-iconvconfig: /dev/null $(objpfx)iconvconfig
	tmp=$(objpfx)gconv-modules.cache.$$$$; \
	rm -f $$tmp; \
	$(make-test-out) --output=$$tmp --nostdlib $(inst_gconvdir) && \
	cmp $$tmp $(inst_gconvdir)/gconv-modules.cache && \
	rm -f $$tmp
