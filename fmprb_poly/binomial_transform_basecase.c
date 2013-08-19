/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_binomial_transform_basecase(fmprb_ptr b, fmprb_srcptr a, long alen, long len, long prec)
{
    long n, k;

    fmpz_t t;
    fmpz_init(t);

    for (n = 0; n < len; n++)
    {
        fmprb_zero(b + n);

        for (k = 0; k < FLINT_MIN(n + 1, alen); k++)
        {
            if (k == 0)
            {
                fmpz_one(t);
            }
            else
            {
                fmpz_mul_si(t, t, -(n - k + 1));
                fmpz_divexact_ui(t, t, k);
            }

            fmprb_addmul_fmpz(b + n, a + k, t, prec);
        }
    }

    fmpz_clear(t);
}

void
fmprb_poly_binomial_transform_basecase(fmprb_poly_t b, const fmprb_poly_t a, long len, long prec)
{
    if (len == 0 || a->length == 0)
    {
        fmprb_poly_zero(b);
        return;
    }

    if (b == a)
    {
        fmprb_poly_t c;
        fmprb_poly_init2(c, len);
        _fmprb_poly_binomial_transform_basecase(c->coeffs, a->coeffs, a->length, len, prec);
        fmprb_poly_swap(b, c);
        fmprb_poly_clear(c);
    }
    else
    {
        fmprb_poly_fit_length(b, len);
        _fmprb_poly_binomial_transform_basecase(b->coeffs, a->coeffs, a->length, len, prec);
    }

    _fmprb_poly_set_length(b, len);
    _fmprb_poly_normalise(b);
}

