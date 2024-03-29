/* Wait on a semaphore with a timeout.  Generic version.
   Copyright (C) 2005, 2006, 2008 Free Software Foundation, Inc.
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

#include <semaphore.h>
#include <errno.h>
#include <assert.h>

#include <pt-internal.h>

int
__sem_timedwait_internal (sem_t *restrict sem,
			  const struct timespec *restrict timeout)
{
  struct __pthread *self;

  __pthread_spin_lock (&sem->__lock);
  if (sem->__value > 0)
    /* Successful down.  */
    {
      sem->__value --;
      __pthread_spin_unlock (&sem->__lock);
      return 0;
    }

  if (timeout && (timeout->tv_nsec < 0 || timeout->tv_nsec >= 1000000000))
    {
      errno = EINVAL;
      return -1;
    }

  /* Add ourselves to the queue.  */
  self = _pthread_self ();

  __pthread_enqueue (&sem->__queue, self);
  __pthread_spin_unlock (&sem->__lock);

  /* Block the thread.  */
  if (timeout)
    {
      error_t err;

      err = __pthread_timedblock (self, timeout, CLOCK_REALTIME);
      if (err)
	/* We timed out.  We may need to disconnect ourself from the
	   waiter queue.

	   FIXME: What do we do if we get a wakeup message before we
	   disconnect ourself?  It may remain until the next time we
	   block.  */
	{
	  assert (err == ETIMEDOUT);

	  __pthread_spin_lock (&sem->__lock);
	  if (self->prevp)
	    __pthread_dequeue (self);
	  __pthread_spin_unlock (&sem->__lock);

	  errno = err;
	  return -1;
	}
    }
  else
    __pthread_block (self);

  return 0;
}

int
__sem_timedwait (sem_t *restrict sem,
		 const struct timespec *restrict timeout)
{
  return __sem_timedwait_internal (sem, timeout);
}

strong_alias (__sem_timedwait, sem_timedwait);
