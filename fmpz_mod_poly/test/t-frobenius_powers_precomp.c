/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2012 Lina Kulakova
    Copyright (C) 2013 Martin Lee

******************************************************************************/

#undef ulong
#define ulong ulongxx/* interferes with system includes */

#include <stdlib.h>
#include <stdio.h>

#undef ulong

#include <gmp.h>

#define ulong mp_limb_t

#include "flint.h"
#include "fmpz_vec.h"
#include "fmpz_mod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    FLINT_TEST_INIT(state);
    

    flint_printf("frobenius_powers_precomp....");
    fflush(stdout);

    /* Aliasing of res and f */
    for (i = 0; i < 250; i++)
    {
        fmpz_mod_poly_t res, f, finv;
        fmpz_mod_poly_frobenius_powers_2exp_t pow;
        fmpz_t p;
        ulong exp;
        
        fmpz_init(p);
        fmpz_set_ui(p, n_randtest_prime(state, 0));
        exp = n_randint(state, 50);
        
        fmpz_mod_poly_init(f, p);
        fmpz_mod_poly_init(finv, p);
        fmpz_mod_poly_init(res, p);

        fmpz_mod_poly_randtest_not_zero(f, state, n_randint(state, 50) + 1);

        fmpz_mod_poly_reverse (finv, f, f->length);
        fmpz_mod_poly_inv_series_newton (finv, finv, f->length);

        fmpz_mod_poly_frobenius_powers_2exp_precomp(pow, f, finv, exp);

        fmpz_mod_poly_frobenius_power(res, pow, f, exp);
        fmpz_mod_poly_frobenius_power(f, pow, f, exp);

        result = (fmpz_mod_poly_equal(res, f));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("f:\n"); fmpz_mod_poly_print(f), flint_printf("\n\n");
            flint_printf("res:\n"); fmpz_mod_poly_print(res), flint_printf("\n\n");
            abort();
        }

        fmpz_clear(p);
        fmpz_mod_poly_clear(f);
        fmpz_mod_poly_clear(finv);
        fmpz_mod_poly_clear(res);
        fmpz_mod_poly_frobenius_powers_2exp_clear(pow);
    }

    /* Compare powers_2exp and powers */
    for (i = 0; i < 250; i++)
    {
        fmpz_mod_poly_t res, f, finv;
        fmpz_mod_poly_frobenius_powers_t pow;
        fmpz_mod_poly_frobenius_powers_2exp_t pow2;
        fmpz_t p;
        ulong exp, exp2;
        
        fmpz_init(p);
        fmpz_set_ui(p, n_randtest_prime(state, 0));
        exp = n_randint(state, 50) + 1;
        exp2 = n_randint(state, exp);

        fmpz_mod_poly_init(f, p);
        fmpz_mod_poly_init(finv, p);
        fmpz_mod_poly_init(res, p);

        fmpz_mod_poly_randtest_not_zero(f, state, n_randint(state, 50) + 1);

        fmpz_mod_poly_reverse (finv, f, f->length);
        fmpz_mod_poly_inv_series_newton (finv, finv, f->length);

        fmpz_mod_poly_frobenius_powers_precomp(pow, f, finv, exp);
        fmpz_mod_poly_frobenius_powers_2exp_precomp(pow2, f, finv, exp);
        
        fmpz_mod_poly_frobenius_power(res, pow2, f, exp2);
        
        result = (fmpz_mod_poly_equal(res, pow->pow + exp2));
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("pow->pow + exp2:\n"); fmpz_mod_poly_print(pow->pow + exp2), flint_printf("\n\n");
            flint_printf("res:\n"); fmpz_mod_poly_print(res), flint_printf("\n\n");
            abort();
        }

        fmpz_clear(p);
        fmpz_mod_poly_clear(f);
        fmpz_mod_poly_clear(finv);
        fmpz_mod_poly_clear(res);
        fmpz_mod_poly_frobenius_powers_clear(pow);
        fmpz_mod_poly_frobenius_powers_2exp_clear(pow2);
    }

    FLINT_TEST_CLEANUP(state);
    
    flint_printf("PASS\n");
    return 0;
}
