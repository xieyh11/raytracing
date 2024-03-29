/* Block a thread with a timeout.  Mach version.
   Copyright (C) 2000, 2002, 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <mach.h>
#include <mach/message.h>

#include <pt-internal.h>

/* Block THREAD.  */
error_t
__pthread_timedblock (struct __pthread *thread,
		      const struct timespec *abstime,
		      clockid_t clock_id)
{
  error_t err;
  mach_msg_header_t msg;
  mach_msg_timeout_t timeout;
  struct timeval now;

  /* We have an absolute time and now we have to convert it to a
     relative time.  Arg.  */

  err = gettimeofday(&now, NULL);
  assert (! err);

  if (now.tv_sec > abstime->tv_sec
      || (now.tv_sec == abstime->tv_sec
	  && now.tv_usec > ((abstime->tv_nsec + 999) / 1000)))
    return ETIMEDOUT;

  timeout = (abstime->tv_sec - now.tv_sec) * 1000;

  if (((abstime->tv_nsec + 999) / 1000) >= now.tv_usec)
    timeout += (((abstime->tv_nsec + 999) / 1000) - now.tv_usec + 999) / 1000;
  else
    /* Need to do a carry.  */
    timeout -= (now.tv_usec + 999) / 1000 -
      ((abstime->tv_nsec + 999999) / 1000000);

  err = __mach_msg (&msg, MACH_RCV_MSG | MACH_RCV_TIMEOUT, 0,
		    sizeof msg, thread->wakeupmsg.msgh_remote_port,
		    timeout, MACH_PORT_NULL);
  if (err == EMACH_RCV_TIMED_OUT)
    return ETIMEDOUT;

  assert_perror (err);
  return 0;
}
