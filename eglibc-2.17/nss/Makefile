# Copyright (C) 1996-1998,2000-2002,2007,2009,2010,2011,2012
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
#	Makefile for name service switch.
#
include ../option-groups.mak

subdir	:= nss

headers			:= nss.h

# These are the databases that go through nss dispatch.
# Caution: if you add a database here, you must add its real name
# in databases.def, too.
databases-y		= grp pwd spwd sgrp
databases-$(OPTION_EGLIBC_INET) \
			+= proto service hosts network rpc ethers \
			   netgrp key
databases-$(OPTION_EGLIBC_DB_ALIASES) += alias

# This is the trivial part which goes into libc itself.
routines-y		+= nsswitch getnssent getnssent_r \
			  $(addsuffix -lookup,$(databases-y))
routines-$(OPTION_EGLIBC_INET) += digits_dots

others                  := getent makedb
install-bin             := getent makedb
makedb-modules = xmalloc hash-string
extra-objs		+= $(makedb-modules:=.o)

tests			= tst-nss-test1
tests-$(OPTION_EGLIBC_INET) += test-netdb
xtests-$(OPTION_EGLIBC_INET) += bug-erange

include ../Makeconfig

# Specify rules for the nss_* modules.  We have some services.
services		:= files

extra-libs		= $(services:%=libnss_%)
# These libraries will be built in the `others' pass rather than
# the `lib' pass, because they depend on libc.so being built already.
extra-libs-others	= $(extra-libs)

# The sources are found in the appropriate subdir.
subdir-dirs = $(services:%=nss_%)
vpath %.c $(subdir-dirs) ../locale/programs ../intl


libnss_files-routines	:= $(addprefix files-,$(databases-y)) \
			   files-initgroups files-have_o_cloexec files-init

libnss_db-dbs		:= $(addprefix db-,\
				       $(filter-out hosts network key alias,\
						    $(databases))) \
			   db-initgroups
libnss_db-routines	:= $(libnss_db-dbs) db-open db-init hash-string
generated		+= $(filter-out db-alias.c db-netgrp.c, \
					$(addsuffix .c,$(libnss_db-dbs)))

install-others		+= $(inst_vardbdir)/Makefile

# Build static module into libc if requested
libnss_files-inhibit-o	= $(filter-out .os,$(object-suffixes))
libnss_db-inhibit-o	= $(filter-out .os,$(object-suffixes))
ifeq ($(build-static-nss),yes)
routines                += $(libnss_files-routines)
static-only-routines    += $(libnss_files-routines)
tests-static		= tst-nss-static
tests			+= $(tests-static)
endif

ifneq ($(OPTION_EGLIBC_NSSWITCH),y)

ifndef OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG
$(error OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG variable left unset)
endif

ifndef OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS
$(error OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS variable left unset)
endif

ifeq (,$(wildcard $(OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG)))
$(warning OPTION_EGLIBC_NSSWITCH is disabled, but fixed config file)
$(error does not exist: $(OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG))
endif

ifeq (,$(wildcard $(OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS)))
$(warning OPTION_EGLIBC_NSSWITCH is disabled, but fixed functions file)
$(error does not exist: $(OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS))
endif

before-compile := $(objpfx)fixed-nsswitch.h
generated := fixed-nsswitch.h
$(objpfx)fixed-nsswitch.h $(objfpx)fixed-nsswitch-libs:	\
    $(objpfx)gen-fixed-nsswitch				\
    $(OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG)
	$< $(objpfx)fixed-nsswitch.h			\
	   $(objpfx)fixed-nsswitch-libs			\
	   $(OPTION_EGLIBC_NSSWITCH_FIXED_CONFIG)

$(objpfx)gen-fixed-nsswitch: gen-fixed-nsswitch.c	\
    $(common-objpfx)option-groups.config		\
    $(OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS)
	$(native-compile)
gen-fixed-nsswitch-CFLAGS =						\
	-g3 -O -Wall							\
	-I $(objpfx)							\
	-DFIXED_FUNCTIONS='"$(OPTION_EGLIBC_NSSWITCH_FIXED_FUNCTIONS)"'
endif

include ../Rules

ifeq (yes,$(have-selinux))
LDLIBS-makedb		:= -lselinux
endif

# Depend on libc.so so a DT_NEEDED is generated in the shared objects.
# This ensures they will load libc.so for needed symbols if loaded by
# a statically-linked program that hasn't already loaded it.
libnss-libc = $(common-objpfx)linkobj/libc.so
$(services:%=$(objpfx)libnss_%.so): $(libnss-libc) \
				    $(common-objpfx)libc_nonshared.a

$(objpfx)libnss_db.so: $(objpfx)libnss_files.so

$(libnss_db-dbs:%=$(objpfx)%.c): $(objpfx)db-%.c: nss_files/files-%.c
	@rm -f $@.new
	(echo '#define EXTERN_PARSER';\
	 echo '#define GENERIC "../nss_db/db-XXX.c"';\
	 echo '#include "$<"') > $@.new
	mv -f $@.new $@


$(objpfx)makedb: $(makedb-modules:%=$(objpfx)%.o)

$(inst_vardbdir)/Makefile: db-Makefile $(+force)
	$(do-install)

CFLAGS-nss_test1.c = -DNOT_IN_libc=1
$(objpfx)/libnss_test1.so: $(objpfx)nss_test1.os $(common-objpfx)libc.so \
			   $(common-objpfx)libc_nonshared.a
	$(build-module)
ifdef libnss_test1.so-version
$(objpfx)/libnss_test1.so$(libnss_test1.so-version): $(objpfx)/libnss_test1.so
	$(make-link)
endif
$(objpfx)tst-nss-test1.out: $(objpfx)/libnss_test1.so$(libnss_test1.so-version)
