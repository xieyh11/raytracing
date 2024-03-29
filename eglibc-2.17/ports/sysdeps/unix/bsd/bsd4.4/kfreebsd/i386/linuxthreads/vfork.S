/* Copyright (C) 1999, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sysdep.h>

/* Clone the calling process, but without copying the whole address space.
   The calling process is suspended until the new process exits or is
   replaced by a call to `execve'.  Return -1 for errors, 0 to the new process,
   and the process ID of the new process to the old process.  */

ENTRY (__vfork)

# ifdef SHARED
	call	GET_PC_THUNK(cx)
	addl	$_GLOBAL_OFFSET_TABLE_, %ecx
	cmpl	$0, __libc_pthread_functions@GOTOFF(%ecx)
# else
	.weak	pthread_create
	movl	$pthread_create, %eax
	testl	%eax, %eax
# endif
	jne	HIDDEN_JUMPTARGET (__fork)

	/* Pop the return PC value into ECX.  */
	popl	%ecx
	cfi_adjust_cfa_offset(-4)

	/* Perform the system call.  */
	DO_CALL (vfork, 0)
	jb	L(error)	/* Branch forward if it failed.  */

	/* In the parent process, %edx == 0, %eax == child pid.  */
	/* In the child process, %edx == 1, %eax == parent pid.  */

	/* Change %eax to be 0 in the child process.  */
	decl	%edx
	andl	%edx, %eax

	/* Jump to the return PC.  */
	jmp	*%ecx

L(error):
	/* Push back the return PC.  */
	pushl	%ecx
	cfi_adjust_cfa_offset(4)

	/* Branch to the error handler, hidden in PSEUDO_END.  */
	jmp	SYSCALL_ERROR_LABEL
L(pseudo_end):
	ret
PSEUDO_END (__vfork)
libc_hidden_def (__vfork)

weak_alias (__vfork, vfork)
