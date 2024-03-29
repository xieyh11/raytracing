# Copyright (C) 1991-2007, 2009, 2011, 2012 Free Software Foundation, Inc.
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
#	Sub-makefile for inet portion of the library.
#
include ../option-groups.mak

subdir	:= inet

headers	:= netinet/ether.h netinet/in.h netinet/in_systm.h \
	   netinet/if_ether.h netinet/igmp.h \
	   netinet/tcp.h netinet/ip.h $(wildcard arpa/*.h protocols/*.h) \
	   aliases.h ifaddrs.h netinet/ip6.h netinet/icmp6.h bits/in.h

routines-$(OPTION_EGLIBC_INET) \
	 += htonl htons \
	    inet_lnaof inet_mkadr	\
	    inet_netof inet_ntoa inet_net herrno herrno-loc \
	    gethstbyad gethstbyad_r gethstbynm gethstbynm2 gethstbynm2_r \
	    gethstbynm_r gethstent gethstent_r \
	    getnetbyad getnetbyad_r getnetbynm getnetent getnetent_r \
	    getnetbynm_r \
	    getproto getproto_r getprtent getprtent_r getprtname getprtname_r \
	    getsrvbynm getsrvbynm_r getsrvbypt getsrvbypt_r getservent \
	    getservent_r	\
	    getrpcent getrpcbyname getrpcbynumber \
	    getrpcent_r getrpcbyname_r getrpcbynumber_r \
	    ether_aton ether_aton_r ether_hton ether_line \
	    ether_ntoa ether_ntoa_r ether_ntoh \
	    getnetgrent_r getnetgrent \
	    in6_addr getnameinfo if_index ifaddrs \
	    getipv4sourcefilter setipv4sourcefilter \
	    getsourcefilter setsourcefilter
routines-$(OPTION_EGLIBC_RCMD) \
	 += rcmd rexec ruserpass
routines-$(OPTION_EGLIBC_DB_ALIASES) \
	 += getaliasent_r getaliasent getaliasname getaliasname_r
routines-$(OPTION_EGLIBC_ADVANCED_INET6) \
	 += inet6_option inet6_opt inet6_rth

aux-$(OPTION_EGLIBC_INET) += check_pf check_native ifreq

tests := htontest test_ifindex tst-ntoa tst-ether_aton tst-network \
	 tst-gethnm test-ifaddrs bug-if1 tst-ether_line \
	 tst-getni1 tst-getni2 tst-inet6_rth tst-checks
tests-$(OPTION_EGLIBC_ADVANCED_INET6) += test-inet6_opt

include ../Rules

ifeq ($(have-thread-library),yes)

CFLAGS-gethstbyad_r.c = -fexceptions
CFLAGS-gethstbyad.c = -fexceptions
CFLAGS-gethstbynm_r.c = -fexceptions
CFLAGS-gethstbynm.c = -fexceptions
CFLAGS-gethstbynm2_r.c = -fexceptions
CFLAGS-gethstbynm2.c = -fexceptions
CFLAGS-gethstent_r.c = -fexceptions
CFLAGS-gethstent.c = -fexceptions
CFLAGS-rcmd.c = -fexceptions
CFLAGS-getnetbynm_r.c = -fexceptions
CFLAGS-getnetbynm.c = -fexceptions
CFLAGS-getnetbyad_r.c = -fexceptions
CFLAGS-getnetbyad.c = -fexceptions
CFLAGS-getnetent_r.c = -fexceptions
CFLAGS-getnetent.c = -fexceptions
CFLAGS-getaliasent_r.c = -fexceptions
CFLAGS-getaliasent.c = -fexceptions
CFLAGS-getrpcent_r.c = -fexceptions
CFLAGS-getrpcent.c = -fexceptions
CFLAGS-getservent_r.c = -fexceptions
CFLAGS-getservent.c = -fexceptions
CFLAGS-getprtent_r.c = -fexceptions
CFLAGS-getprtent.c = -fexceptions
CFLAGS-either_ntoh.c = -fexceptions
CFLAGS-either_hton.c = -fexceptions
CFLAGS-getnetgrent.c = -fexceptions
CFLAGS-getnetgrent_r.c = -fexceptions

endif

ifeq ($(build-static-nss),yes)
CFLAGS += -DSTATIC_NSS
endif
