/* low level locking for pthread library.  Generic futex-using version.
   Copyright (C) 2003, 2007 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Paul Mackerras <paulus@au.ibm.com>, 2003.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <sysdep.h>
#include <lowlevellock.h>
#include <sys/time.h>
#include <tls.h>		/* Need THREAD_*, and header.*.  */

void
__lll_lock_wait (lll_lock_t *futex, int private)
{
  do
    {
      int oldval = atomic_compare_and_exchange_val_acq (futex, 2, 1);
      if (oldval != 0)
	lll_futex_wait (futex, 2, private);
    }
  while (atomic_compare_and_exchange_val_acq (futex, 2, 0) != 0);
}

void
__lll_lock_wait_private (lll_lock_t *futex)
{
  do
    {
      int oldval = atomic_compare_and_exchange_val_acq (futex, 2, 1);
      if (oldval != 0)
	lll_futex_wait (futex, 2, LLL_PRIVATE);
    }
  while (atomic_compare_and_exchange_val_acq (futex, 2, 0) != 0);
}

int
__lll_timedlock_wait (lll_lock_t *futex, const struct timespec *abstime, int private)
{
  /* Reject invalid timeouts.  */
  if (abstime->tv_nsec < 0 || abstime->tv_nsec >= 1000000000)
    return EINVAL;

  do
    {
      struct timeval tv;
      struct timespec rt;

      /* Get the current time.  */
      (void) __gettimeofday (&tv, NULL);

      /* Compute relative timeout.  */
      rt.tv_sec = abstime->tv_sec - tv.tv_sec;
      rt.tv_nsec = abstime->tv_nsec - tv.tv_usec * 1000;
      if (rt.tv_nsec < 0)
	{
	  rt.tv_nsec += 1000000000;
	  --rt.tv_sec;
	}

      /* Already timed out?  */
      if (rt.tv_sec < 0)
	return ETIMEDOUT;

      /* Wait.  */
      int oldval = atomic_compare_and_exchange_val_acq (futex, 2, 1);
      if (oldval != 0)
	lll_futex_timed_wait (futex, 2, &rt, private);
    }
  while (atomic_compare_and_exchange_bool_acq (futex, 2, 0) != 0);
  return 0;
}


/* These don't get included in libc.so  */
#ifdef IS_IN_libpthread
int
lll_unlock_wake_cb (lll_lock_t *futex)
{
  int val = atomic_exchange_rel (futex, 0);

  if (__builtin_expect (val > 1, 0))
    lll_private_futex_wake (futex, 1);
  return 0;
}


int
__lll_timedwait_tid (int *tidp, const struct timespec *abstime)
{
  int tid;

  if (abstime->tv_nsec < 0 || abstime->tv_nsec >= 1000000000)
    return EINVAL;

  /* Repeat until thread terminated.  */
  while ((tid = *tidp) != 0)
    {
      struct timeval tv;
      struct timespec rt;

      /* Get the current time.  */
      (void) __gettimeofday (&tv, NULL);

      /* Compute relative timeout.  */
      rt.tv_sec = abstime->tv_sec - tv.tv_sec;
      rt.tv_nsec = abstime->tv_nsec - tv.tv_usec * 1000;
      if (rt.tv_nsec < 0)
	{
	  rt.tv_nsec += 1000000000;
	  --rt.tv_sec;
	}

      /* Already timed out?  */
      if (rt.tv_sec < 0)
	return ETIMEDOUT;

      /* Wait until thread terminates.  */
      if (lll_futex_timed_wait (tidp, tid, &rt, LLL_SHARED) == -ETIMEDOUT)
	return ETIMEDOUT;
    }

  return 0;
}

#endif

