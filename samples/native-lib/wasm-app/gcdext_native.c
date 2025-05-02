/* gcd.c -- benchmark gcd.

Copyright 2003, 2009 Free Software Foundation, Inc.

This file is part of GMPbench.

GMPbench is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

GMPbench is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
GMPbench.  If not, see http://www.gnu.org/licenses/.  */

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "timing.h"

#include "nativefunc.h"

int
cputime(void);

int
GcdextTest(int M, int N)
{
    gmp_randstate_t rs;
    mpz_t x, y, z, s;
    unsigned long int m, n, i, niter, t0, ti;
    double t, f, ops_per_sec;
    int decimals;

    //   if (argc != 3)
    //     {
    //       fprintf (stderr, "usage: %s m n\n", argv[0]);
    //       fprintf (stderr, "  where m and n are number of bits in numbers
    //       tested\n"); return -1;
    //     }

    m = M;
    n = N;

    gmp_randinit_default(rs);

    mpz_init(x);
    mpz_init(y);
    mpz_init(z);
    mpz_init(s);
    mpz_urandomb(x, rs, m);
    mpz_urandomb(y, rs, n);

    printf("Calibrating CPU speed...");
    fflush(stdout);
    TIME(t, mpz_gcdext(z, s, NULL, x, y));
    printf("done\n");

    niter = 1 + (unsigned long)(1e4 / t);
    printf("Multiplying %lu-bit number with %lu-bit number %lu times...", m, n,
           niter);
    fflush(stdout);
    t0 = cputime();
    for (i = niter; i > 0; i--) {
        mpz_gcdext(z, s, NULL, x, y);
    }
    ti = cputime() - t0;
    printf("done!\n");

    ops_per_sec = 1000.0 * niter / ti;
    f = 100.0;
    for (decimals = 0;; decimals++) {
        if (ops_per_sec > f)
            break;
        f = f * 0.1;
    }

    printf("RESULT: %.*f operations per second\n", decimals, ops_per_sec);
    return 0;
}

/* Return user CPU time measured in milliseconds.  */
#if !defined(__sun)                                         \
    && (defined(USG) || defined(__SVR4) || defined(_UNICOS) \
        || defined(__hpux))
#include <time.h>

int
cputime()
{
    return (int)((double)clock() * 1000 / CLOCKS_PER_SEC);
}
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <time.h>

int
cputime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // プロセスの経過時間を取得
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
// int
// cputime ()
// {
//   struct rusage rus;

//   getrusage (0, &rus);
//   return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
// }
#endif

int
main()
{
    printf("インタプリタ+ネイティブ gcdext_native.c\n");
    // gcdext_args="128,128 512,512 8192,8192 131072,131072 1048576,1048576"

    int b1[] = { 128, 512, 8192, 131072, 1048576 };
    int b2[] = { 128, 512, 8192, 131072, 1048576 };

    for (int i = 0; i < 5; i++) {
        GcdextTest(b1[i], b2[i]);
        printf("\n");
    }

    return 0;
}