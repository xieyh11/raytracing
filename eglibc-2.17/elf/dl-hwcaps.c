/* Hardware capability support for run-time dynamic loader.
   Copyright (C) 2012 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <libintl.h>
#include <unistd.h>
#include <ldsodefs.h>

#include <dl-procinfo.h>

#ifdef _DL_FIRST_PLATFORM
# define _DL_FIRST_EXTRA (_DL_FIRST_PLATFORM + _DL_PLATFORMS_COUNT)
#else
# define _DL_FIRST_EXTRA _DL_HWCAP_COUNT
#endif

/* Return an array of useful/necessary hardware capability names.  */
const struct r_strlenpair *
internal_function
_dl_important_hwcaps (const char *platform, size_t platform_len, size_t *sz,
		      size_t *max_capstrlen)
{
  /* Determine how many important bits are set.  */
  uint64_t masked = GLRO(dl_hwcap) & GLRO(dl_hwcap_mask);
  size_t cnt = platform != NULL;
  size_t n, m;
  size_t total;
  struct r_strlenpair *temp;
  struct r_strlenpair *result;
  struct r_strlenpair *rp;
  char *cp;

  /* Count the number of bits set in the masked value.  */
  for (n = 0; (~((1ULL << n) - 1) & masked) != 0; ++n)
    if ((masked & (1ULL << n)) != 0)
      ++cnt;

#if defined NEED_DL_SYSINFO || defined NEED_DL_SYSINFO_DSO
  /* The system-supplied DSO can contain a note of type 2, vendor "GNU".
     This gives us a list of names to treat as fake hwcap bits.  */

  const char *dsocaps = NULL;
  size_t dsocapslen = 0;
  if (GLRO(dl_sysinfo_map) != NULL)
    {
      const ElfW(Phdr) *const phdr = GLRO(dl_sysinfo_map)->l_phdr;
      const ElfW(Word) phnum = GLRO(dl_sysinfo_map)->l_phnum;
      for (uint_fast16_t i = 0; i < phnum; ++i)
	if (phdr[i].p_type == PT_NOTE)
	  {
	    const ElfW(Addr) start = (phdr[i].p_vaddr
				      + GLRO(dl_sysinfo_map)->l_addr);
	    const struct
	    {
	      ElfW(Word) vendorlen;
	      ElfW(Word) datalen;
	      ElfW(Word) type;
	    } *note = (const void *) start;
	    while ((ElfW(Addr)) (note + 1) - start < phdr[i].p_memsz)
	      {
#define ROUND(len) (((len) + sizeof (ElfW(Word)) - 1) & -sizeof (ElfW(Word)))
		if (note->type == NT_GNU_HWCAP
		    && note->vendorlen == sizeof "GNU"
		    && !memcmp ((note + 1), "GNU", sizeof "GNU")
		    && note->datalen > 2 * sizeof (ElfW(Word)) + 2)
		  {
		    const ElfW(Word) *p = ((const void *) (note + 1)
					   + ROUND (sizeof "GNU"));
		    cnt += *p++;
		    ++p;	/* Skip mask word.  */
		    dsocaps = (const char *) p;
		    dsocapslen = note->datalen - sizeof *p * 2;
		    break;
		  }
		note = ((const void *) (note + 1)
			+ ROUND (note->vendorlen) + ROUND (note->datalen));
#undef ROUND
	      }
	    if (dsocaps != NULL)
	      break;
	  }
    }
#endif

  /* For TLS enabled builds always add 'tls'.  */
  ++cnt;

  if (__access ("/etc/ld.so.nohwcap", F_OK) == 0)
    {
      /* If hwcap is disabled, we only have the base directory to search.  */
      result = (struct r_strlenpair *) malloc (sizeof (*result));
      if (result == NULL)
	goto no_memory;

      result[0].str = (char *) result;  /* Does not really matter.  */
      result[0].len = 0;

      *sz = 1;
      return result;
    }

  /* Create temporary data structure to generate result table.  */
  temp = (struct r_strlenpair *) alloca (cnt * sizeof (*temp));
  m = 0;
#if defined NEED_DL_SYSINFO || defined NEED_DL_SYSINFO_DSO
  if (dsocaps != NULL)
    {
      const ElfW(Word) mask = ((const ElfW(Word) *) dsocaps)[-1];
      GLRO(dl_hwcap) |= (uint64_t) mask << _DL_FIRST_EXTRA;
      /* Note that we add the dsocaps to the set already chosen by the
	 LD_HWCAP_MASK environment variable (or default HWCAP_IMPORTANT).
	 So there is no way to request ignoring an OS-supplied dsocap
	 string and bit like you can ignore an OS-supplied HWCAP bit.  */
      GLRO(dl_hwcap_mask) |= (uint64_t) mask << _DL_FIRST_EXTRA;
      size_t len;
      for (const char *p = dsocaps; p < dsocaps + dsocapslen; p += len + 1)
	{
	  uint_fast8_t bit = *p++;
	  len = strlen (p);

	  /* Skip entries that are not enabled in the mask word.  */
	  if (__builtin_expect (mask & ((ElfW(Word)) 1 << bit), 1))
	    {
	      temp[m].str = p;
	      temp[m].len = len;
	      ++m;
	    }
	  else
	    --cnt;
	}
    }
#endif
  for (n = 0; masked != 0; ++n)
    if ((masked & (1ULL << n)) != 0)
      {
	temp[m].str = _dl_hwcap_string (n);
	temp[m].len = strlen (temp[m].str);
	masked ^= 1ULL << n;
	++m;
      }
  if (platform != NULL)
    {
      temp[m].str = platform;
      temp[m].len = platform_len;
      ++m;
    }

  temp[m].str = "tls";
  temp[m].len = 3;
  ++m;

  assert (m == cnt);

  /* Determine the total size of all strings together.  */
  if (cnt == 1)
    total = temp[0].len + 1;
  else
    {
      total = temp[0].len + temp[cnt - 1].len + 2;
      if (cnt > 2)
	{
	  total <<= 1;
	  for (n = 1; n + 1 < cnt; ++n)
	    total += temp[n].len + 1;
	  if (cnt > 3
	      && (cnt >= sizeof (size_t) * 8
		  || total + (sizeof (*result) << 3)
		     >= (1UL << (sizeof (size_t) * 8 - cnt + 3))))
	    _dl_signal_error (ENOMEM, NULL, NULL,
			      N_("cannot create capability list"));

	  total <<= cnt - 3;
	}
    }

  /* The result structure: we use a very compressed way to store the
     various combinations of capability names.  */
  *sz = 1 << cnt;
  result = (struct r_strlenpair *) malloc (*sz * sizeof (*result) + total);
  if (result == NULL)
    {
    no_memory:
      _dl_signal_error (ENOMEM, NULL, NULL,
		     	N_("cannot create capability list"));
    }

  if (cnt == 1)
    {
      result[0].str = (char *) (result + *sz);
      result[0].len = temp[0].len + 1;
      result[1].str = (char *) (result + *sz);
      result[1].len = 0;
      cp = __mempcpy ((char *) (result + *sz), temp[0].str, temp[0].len);
      *cp = '/';
      *sz = 2;
      *max_capstrlen = result[0].len;

      return result;
    }

  /* Fill in the information.  This follows the following scheme
     (indeces from TEMP for four strings):
	entry #0: 0, 1, 2, 3	binary: 1111
	      #1: 0, 1, 3		1101
	      #2: 0, 2, 3		1011
	      #3: 0, 3			1001
     This allows the representation of all possible combinations of
     capability names in the string.  First generate the strings.  */
  result[1].str = result[0].str = cp = (char *) (result + *sz);
#define add(idx) \
      cp = __mempcpy (__mempcpy (cp, temp[idx].str, temp[idx].len), "/", 1);
  if (cnt == 2)
    {
      add (1);
      add (0);
    }
  else
    {
      n = 1 << (cnt - 1);
      do
	{
	  n -= 2;

	  /* We always add the last string.  */
	  add (cnt - 1);

	  /* Add the strings which have the bit set in N.  */
	  for (m = cnt - 2; m > 0; --m)
	    if ((n & (1 << m)) != 0)
	      add (m);

	  /* Always add the first string.  */
	  add (0);
	}
      while (n != 0);
    }
#undef add

  /* Now we are ready to install the string pointers and length.  */
  for (n = 0; n < (1UL << cnt); ++n)
    result[n].len = 0;
  n = cnt;
  do
    {
      size_t mask = 1 << --n;

      rp = result;
      for (m = 1 << cnt; m > 0; ++rp)
	if ((--m & mask) != 0)
	  rp->len += temp[n].len + 1;
    }
  while (n != 0);

  /* The first half of the strings all include the first string.  */
  n = (1 << cnt) - 2;
  rp = &result[2];
  while (n != (1UL << (cnt - 1)))
    {
      if ((--n & 1) != 0)
	rp[0].str = rp[-2].str + rp[-2].len;
      else
	rp[0].str = rp[-1].str;
      ++rp;
    }

  /* The second half starts right after the first part of the string of
     the corresponding entry in the first half.  */
  do
    {
      rp[0].str = rp[-(1 << (cnt - 1))].str + temp[cnt - 1].len + 1;
      ++rp;
    }
  while (--n != 0);

  /* The maximum string length.  */
  *max_capstrlen = result[0].len;

  return result;
}
