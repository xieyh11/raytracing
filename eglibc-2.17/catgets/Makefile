# Copyright (C) 1996-2012 Free Software Foundation, Inc.
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
#	Makefile for catgets.
#
subdir	:= catgets

include ../option-groups.mak

headers		= nl_types.h
routines-$(OPTION_EGLIBC_CATGETS)    := catgets open_catalog
others-$(OPTION_EGLIBC_CATGETS)      := gencat
install-bin-$(OPTION_EGLIBC_CATGETS) := gencat
extra-objs-$(OPTION_EGLIBC_CATGETS)  := $(gencat-modules:=.o)

tests-$(OPTION_EGLIBC_CATGETS)       := tst-catgets
test-srcs-$(OPTION_EGLIBC_CATGETS)   := test-gencat

gencat-modules	= xmalloc

# To find xmalloc.c
vpath %.c ../locale/programs


include ../Rules

$(objpfx)gencat: $(gencat-modules:%=$(objpfx)%.o)

catgets-CPPFLAGS := -DNLSPATH='"$(msgcatdir)/%L/%N:$(msgcatdir)/%L/LC_MESSAGES/%N:$(msgcatdir)/%l/%N:$(msgcatdir)/%l/LC_MESSAGES/%N:"' \
	    -DHAVE_CONFIG_H

CPPFLAGS-gencat = -DNOT_IN_libc

generated = de.msg test1.cat test1.h test2.cat test2.h sample.SJIS.cat \
	    test-gencat.h
generated-dirs = de

tst-catgets-ENV = NLSPATH="$(objpfx)%l/%N.cat" LANG=de

ifeq ($(run-built-tests),yes)
ifeq (y,$(OPTION_EGLIBC_CATGETS))
tests: $(objpfx)de/libc.cat $(objpfx)test1.cat $(objpfx)test2.cat \
       $(objpfx)test-gencat.out
endif
# This test just checks whether the program produces any error or not.
# The result is not tested.
$(objpfx)test1.cat: test1.msg $(objpfx)gencat
	LC_ALL=hr_HR.ISO-8859-2 LOCPATH=$(common-objpfx)localedata \
	GCONV_PATH=$(common-objpfx)iconvdata \
	$(built-program-cmd) -H $(objpfx)test1.h $@ $<
$(objpfx)test2.cat: test2.msg $(objpfx)gencat
	LOCPATH=$(common-objpfx)localedata \
	GCONV_PATH=$(common-objpfx)iconvdata \
	$(built-program-cmd) -H $(objpfx)test2.h $@ $<
$(objpfx)de/libc.cat: $(objpfx)de.msg $(objpfx)gencat
	$(make-target-directory)
	LC_ALL=de_DE.ISO-8859-1 LOCPATH=$(common-objpfx)localedata \
	GCONV_PATH=$(common-objpfx)iconvdata $(built-program-cmd) $@ $<
$(objpfx)tst-catgets.out: $(objpfx)de/libc.cat

# Generate a non-simple input file.
$(objpfx)de.msg: xopen-msg.awk $(..)po/de.po
	LC_ALL=C $(AWK) -f $^ $< > $@

$(objpfx)test-gencat.out: test-gencat.sh $(objpfx)test-gencat \
			  $(objpfx)sample.SJIS.cat
	$(SHELL) $< $(common-objpfx) '$(built-program-cmd)'

$(objpfx)sample.SJIS.cat: sample.SJIS $(objpfx)gencat
	GCONV_PATH=$(common-objpfx)iconvdata LC_ALL=C \
	$(built-program-cmd) -H $(objpfx)test-gencat.h < $(word 1,$^) > $@
endif
