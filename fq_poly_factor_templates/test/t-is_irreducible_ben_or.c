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

    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2012 Lina Kulakova
    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz_vec.h"
#include "ulong_extras.h"
int
main(void)
{
    int iter;
    FLINT_TEST_INIT(state);

    flint_printf("is_irreducible_ben_or....");
    fflush(stdout);

    for (iter = 0; iter < 50; iter++)
    {
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) poly1, poly2;
        slong length;
        int i, num;

        TEMPLATE(T, ctx_randtest) (ctx, state);

        TEMPLATE(T, poly_init) (poly1, ctx);
        TEMPLATE(T, poly_init) (poly2, ctx);

        length = n_randint(state, 7) + 2;
        do
        {
            TEMPLATE(T, poly_randtest) (poly1, state, length, ctx);
            if (!TEMPLATE(T, poly_is_zero) (poly1, ctx))
                TEMPLATE(T, poly_make_monic) (poly1, poly1, ctx);
        }
        while ((!TEMPLATE(T, poly_is_irreducible_ben_or) (poly1, ctx))
               || (poly1->length < 2));

        num = n_randint(state, 5) + 1;

        for (i = 0; i < num; i++)
        {
            do
            {
                TEMPLATE(T, poly_randtest) (poly2, state, length, ctx);
                if (!TEMPLATE(T, poly_is_zero) (poly2, ctx))
                    TEMPLATE(T, poly_make_monic) (poly2, poly2, ctx);
            }
            while ((!TEMPLATE(T, poly_is_irreducible_ben_or) (poly2, ctx))
                   || (poly2->length < 2));

            TEMPLATE(T, poly_mul) (poly1, poly1, poly2, ctx);
        }

        if (TEMPLATE(T, poly_is_irreducible_ben_or) (poly1, ctx))
        {
            flint_printf
                ("Error: reducible polynomial declared irreducible!\n");
            flint_printf("poly:\n");
            TEMPLATE(T, poly_print) (poly1, ctx);
            flint_printf("\n");
            abort();
        }

        TEMPLATE(T, poly_clear) (poly1, ctx);
        TEMPLATE(T, poly_clear) (poly2, ctx);

        TEMPLATE(T, ctx_clear) (ctx);
    }

    FLINT_TEST_CLEANUP(state);
    flint_printf("PASS\n");
    return 0;
}


#endif
