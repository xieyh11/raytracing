/* s_logbl.c -- long double version of s_logb.c.
 * Conversion to IEEE quad long double by Jakub Jelinek, jj@ultra.linux.cz.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * long double logbl(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#include <math.h>
#include <math_private.h>
#include <math_ldbl_opt.h>

long double
__logbl (long double x)
{
  int64_t hx, rhx;
  int64_t lx __attribute__ ((unused));
  long double ret;

  GET_LDOUBLE_WORDS64 (hx, lx, x);
  hx &= 0x7fffffffffffffffLL;	/* high |x| */
  if (hx == 0)
    return -1.0 / fabs (x);
  if (hx >= 0x7ff0000000000000LL)
    return x * x;
  if (__builtin_expect ((rhx = hx >> 52) == 0, 0))
    {
      /* POSIX specifies that denormal number is treated as
         though it were normalized.  */
      rhx -= __builtin_clzll (hx) - 12;
    }
  ret = (long double) (rhx - 1023);
  /* The test is to avoid logb_downward (0.0) == -0.0.  */
  return ret == -0.0 ? 0.0 : ret;
}

long_double_symbol (libm, __logbl, logbl);
