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
#	Makefile for stdlib routines
#
include ../option-groups.mak

subdir	:= stdlib

headers	:= stdlib.h bits/stdlib.h bits/stdlib-ldbl.h bits/stdlib-float.h      \
	   monetary.h bits/monetary-ldbl.h				      \
	   inttypes.h stdint.h bits/wordsize.h				      \
	   errno.h sys/errno.h bits/errno.h				      \
	   ucontext.h sys/ucontext.h					      \
	   alloca.h fmtmsg.h

routines-y	:=							      \
	atof atoi atol atoll						      \
	abort								      \
	bsearch qsort msort						      \
	getenv putenv setenv secure-getenv				      \
	exit on_exit atexit cxa_atexit cxa_finalize old_atexit		      \
	quick_exit at_quick_exit cxa_at_quick_exit			      \
	abs labs llabs							      \
	div ldiv lldiv							      \
	random random_r rand rand_r					      \
	drand48 erand48 lrand48 nrand48 mrand48 jrand48			      \
	srand48 seed48 lcong48						      \
	drand48_r erand48_r lrand48_r nrand48_r mrand48_r jrand48_r	      \
	srand48_r seed48_r lcong48_r					      \
	drand48-iter							      \
	strtol strtoul strtoll strtoull					      \
	strtol_l strtoul_l strtoll_l strtoull_l				      \
	strtof strtod strtold						      \
	strtof_l strtod_l strtold_l					      \
	system canonicalize						      \
	a64l l64a							      \
	getsubopt xpg_basename						      \
	strtoimax strtoumax						      \
	getcontext setcontext makecontext swapcontext
routines-$(OPTION_EGLIBC_LOCALE_CODE) +=				      \
	strfmon strfmon_l
routines-$(OPTION_POSIX_C_LANG_WIDE_CHAR) +=				      \
	mblen mbstowcs mbtowc wcstombs wctomb				      \
	wcstoimax wcstoumax
ifeq (yy,$(OPTION_EGLIBC_LOCALE_CODE)$(OPTION_POSIX_REGEXP))
routines-y += rpmatch
endif
routines-$(OPTION_EGLIBC_FMTMSG) += fmtmsg
aux =	grouping groupingwc tens_in_limb

# These routines will be omitted from the libc shared object.
# Instead the static object files will be included in a special archive
# linked against when the shared library will be used.
static-only-routines = atexit at_quick_exit

test-srcs-$(OPTION_EGLIBC_FMTMSG) := tst-fmtmsg
tests		:= tst-strtol tst-strtod testrand testsort testdiv          \
		   test-canon test-canon2 tst-strtoll tst-environ	    \
		   tst-xpg-basename tst-random tst-random2 tst-bsearch	    \
		   tst-limits tst-rand48 bug-strtod tst-setcontext	    \
		   test-a64l tst-qsort tst-system bug-strtod2		    \
		   tst-atof1 tst-atof2 tst-strtod2 tst-rand48-2             \
		   tst-makecontext tst-qsort2 tst-makecontext2 tst-strtod6  \
		   tst-unsetenv1 tst-makecontext3 bug-getcontext bug-fmtmsg1 \
		   tst-secure-getenv tst-strtod-overflow tst-strtod-round   \
		   tst-tininess tst-strtod-underflow
tests-static	:= tst-secure-getenv
tests-$(OPTION_EGLIBC_LOCALE_CODE) \
		+= tst-strtod3 tst-strtod4 tst-strtod5 testmb2
tests-$(OPTION_POSIX_C_LANG_WIDE_CHAR) \
		+= testmb
include ../Makeconfig

ifeq ($(build-shared),yes)
tests		+= tst-putenv
endif

# Several mpn functions from GNU MP are used by the strtod function.
mpn-routines := inlines add_n addmul_1 cmp divmod_1 divrem udiv_qrnnd \
		lshift rshift mod_1 mul mul_1 mul_n sub_n submul_1
mpn-headers = longlong.h gmp.h gmp-impl.h gmp-mparam.h asm-syntax.h

routines := $(strip $(routines) $(mpn-routines))	\
	    dbl2mpn ldbl2mpn				\
	    mpn2flt mpn2dbl mpn2ldbl
aux += fpioconst mp_clz_tab

tests-extras += tst-putenvmod
extra-test-objs += tst-putenvmod.os

generated += isomac isomac.out tst-putenvmod.so

CFLAGS-bsearch.c = $(uses-callbacks)
CFLAGS-msort.c = $(uses-callbacks)
CFLAGS-qsort.c = $(uses-callbacks)
CFLAGS-system.c = -fexceptions
CFLAGS-system.os = -fomit-frame-pointer
CFLAGS-fmtmsg.c = -fexceptions

CFLAGS-strfmon.c = $(libio-mtsafe)
CFLAGS-strfmon_l.c = $(libio-mtsafe)

CFLAGS-tst-bsearch.c = $(stack-align-test-flags)
CFLAGS-tst-qsort.c = $(stack-align-test-flags)
CFLAGS-tst-makecontext2.c = $(stack-align-test-flags)

include ../Rules

# Testdir has to be named stdlib and needs to be writable
test-canon-ARGS = --test-dir=${common-objpfx}stdlib

tst-strtod-ENV = LOCPATH=$(common-objpfx)localedata
tst-strtod3-ENV = LOCPATH=$(common-objpfx)localedata
tst-strtod4-ENV = LOCPATH=$(common-objpfx)localedata
tst-strtod5-ENV = LOCPATH=$(common-objpfx)localedata
testmb2-ENV = LOCPATH=$(common-objpfx)localedata
bug-fmtmsg1-ENV = SEV_LEVEL=foo,11,newsev

# Run a test on the header files we use.
tests: $(objpfx)isomac.out

ifeq ($(run-built-tests),yes)
ifeq (y,$(OPTION_EGLIBC_FMTMSG))
tests: $(objpfx)tst-fmtmsg.out
endif
endif

$(objpfx)isomac.out: $(objpfx)isomac
	$(dir $<)$(notdir $<) '$(CC)'  \
	'-I../include $(+sysdep-includes) $(sysincludes) -I..' > $<.out

isomac-CFLAGS = -O
$(objpfx)isomac: isomac.c
	$(native-compile)

$(objpfx)tst-fmtmsg.out: tst-fmtmsg.sh $(objpfx)tst-fmtmsg
	$(SHELL) $< $(common-objpfx) '$(run-program-prefix)' $(common-objpfx)stdlib/

$(objpfx)tst-putenv: $(objpfx)tst-putenvmod.so
LDFLAGS-tst-putenv = $(no-as-needed)

$(objpfx)tst-putenvmod.so: $(objpfx)tst-putenvmod.os
	$(build-module)
CFLAGS-tst-putenvmod.c = -DNOT_IN_libc=1

ifeq ($(build-shared),yes)
link-libm = $(common-objpfx)math/libm.so
else
link-libm = $(common-objpfx)math/libm.a
endif
$(objpfx)bug-getcontext: $(link-libm)
$(objpfx)tst-strtod-round: $(link-libm)
$(objpfx)tst-tininess: $(link-libm)
$(objpfx)tst-strtod-underflow: $(link-libm)
