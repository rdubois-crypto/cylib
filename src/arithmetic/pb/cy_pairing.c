/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project */
/* License: This software is licensed under a dual BSD and GPL v2 license */
/* See LICENSE file at the root folder of the project. */
/*                                                                                */
/* 																			      */
/* FILE: cy_pairing.c */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bls12 pairings   */
/**********************************************************************************/


/*
 * Copyright Supranational LLC
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */


#include "cy_fp.h"
#include "cy_fp2.h"
#include "cy_fp12.h"

/*
 * Line evaluations from  https://eprint.iacr.org/2010/354.pdf
 * with a twist moving common expression to line_by_Px2.
 */
static cy_error_t cy_line_add(cy_fp6_t line, POINTonE2 *T, const POINTonE2 *R,
                                                   const POINTonE2_affine *Q)
{
    /*vec384x Z1Z1, U2, S2, H, HH, I, J, V;*/
	cy_fp2_t *Z1Z1, *U2, *S2, *H, *HH, *I, *J, *V;

	/* todo: allocate*/
#if 1
# define r line[1]
#else
   /* vec384x r;*/
	cy_fp2_t r;
#endif

    /*
     * https://hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-madd-2007-bl
     * with XYZ3 being |T|, XYZ1 - |R|, XY2 - |Q|, i.e. Q is affine
     */
//    sqr_fp2(Z1Z1, R->Z);                /* Z1Z1 = Z1^2 */
	cy_fp2_sqr(R->Z, Z1Z1);
//    mul_fp2(U2, Q->X, Z1Z1);            /* U2 = X2*Z1Z1 */
	cy_fp2_mul( Z1Z1, Q->X, Z1Z1);

//    mul_fp2(S2, Q->Y, R->Z);
	cy_fp2_mul(  Q->Y, R->Z, S2);

//    mul_fp2(S2, S2, Z1Z1);              /* S2 = Y2*Z1*Z1Z1 */
	cy_fp2_mul(  S2, Z1Z1, S2);

//    sub_fp2(H, U2, R->X);               /* H = U2-X1 */
	cy_fp2_sub(  U2, R->X, H);

//    sqr_fp2(HH, H);                     /* HH = H^2 */
	cy_fp2_sqr(  H, HH);


//    add_fp2(I, HH, HH);
	cy_fp2_add(  HH, HH, I);

//    add_fp2(I, I, I);                   /* I = 4*HH */
	cy_fp2_add(  I, I, I);

//    mul_fp2(J, H, I);                   /* J = H*I */
	cy_fp2_mul(  H, I, J);

//    sub_fp2(r, S2, R->Y);
	cy_fp2_sub(S2, R->Y, r);

//    add_fp2(r, r, r);                   /* r = 2*(S2-Y1) */
	cy_fp2_add(r,r,r);

//    mul_fp2(V, R->X, I);                /* V = X1*I */
	cy_fp2_mul(I,R->X,V);

//    sqr_fp2(T->X, r);
	cy_fp2_sqr(r,T->X);

//    sub_fp2(T->X, T->X, J);
	cy_fp2_sub( T->X, J, T->X);

//    sub_fp2(T->X, T->X, V);
	cy_fp2_sub(T->X, V, T->X);
//    sub_fp2(T->X, T->X, V);             /* X3 = r^2-J-2*V */
	cy_fp2_sub(T->X, V, T->X);
//TODO
//    mul_fp2(J, J, R->Y);
	cy_fp2_mul(J, R->Y, J);

//    sub_fp2(T->Y, V, T->X);
	cy_fp2_sub(V, T->X, T->Y);

//    mul_fp2(T->Y, T->Y, r);
	cy_fp2_mul(T->Y, r, T->Y);

//    sub_fp2(T->Y, T->Y, J);
	cy_fp2_sub(T->Y, J, T->Y);
//    sub_fp2(T->Y, T->Y, J);             /* Y3 = r*(V-X3)-2*Y1*J */
	cy_fp2_sub(T->Y, J, T->Y);

//    add_fp2(T->Z, R->Z, H);
	cy_fp2_add(R->Z, H, T->Z);

    //sqr_fp2(T->Z, T->Z);
	cy_fp2_add(T->Z, T->Z);

    //sub_fp2(T->Z, T->Z, Z1Z1);
	cy_fp2_sub(T->Z, Z1Z1, T->Z);

//    sub_fp2(T->Z, T->Z, HH);            /* Z3 = (Z1+H)^2-Z1Z1-HH */
	cy_fp2_sub(T->Z, HH, T->Z);

    /*
     * line evaluation
     */
//    mul_fp2(I, r, Q->X);
	cy_fp2_mul(r, Q->X,I);

//    mul_fp2(J, Q->Y, T->Z);
	cy_fp2_mul(Q->Y, T->Z, J);

//    sub_fp2(I, I, J);
	cy_fp2_mul(I, J, I);

//    add_fp2(line[0], I, I);          /* 2*(r*X2 - Y2*Z3) */
	cy_fp2_add(I, I, line[0]);

	//TODO
#ifdef r
# undef r
#else
    vec_copy(line[1], r, sizeof(r));
#endif
    vec_copy(line[2], T->Z, sizeof(T->Z));
}

static void line_dbl(vec384fp6 line, POINTonE2 *T, const POINTonE2 *Q)
{
    cy_fp2_t *ZZ, *A, *B, *C, *D, *E, *F;

    /*
     * https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#doubling-dbl-2009-alnr
     */
//    sqr_fp2(A, Q->X);                   /* A = X1^2 */
    cy_fp2_sqr(Q->X, A);

//    sqr_fp2(B, Q->Y);                   /* B = Y1^2 */
    cy_fp2_sqr(Q->Y, B);

//    sqr_fp2(ZZ, Q->Z);                  /* ZZ = Z1^2 */
    cy_fp2_sqr(Q->Z, ZZ);

//    sqr_fp2(C, B);                      /* C = B^2 */
    cy_fp2_sqr(B,C);

//    add_fp2(D, Q->X, B);                /* X1+B */
    cy_fp2_add(Q->X, B, D);

 //   sqr_fp2(D, D);                      /* (X1+B)^2 */
    cy_fp2_sqr(D, D);

 //   sub_fp2(D, D, A);                   /* (X1+B)^2-A */
    cy_fp2_sub(D, A, D);

//    sub_fp2(D, D, C);                   /* (X1+B)^2-A-C */
    cy_fp2_sub(D, C, D);

//    add_fp2(D, D, D);                   /* D = 2*((X1+B)^2-A-C) */
    cy_fp2_add(D, D, D);

    mul_by_3_fp2(E, A);                 /* E = 3*A */
//    sqr_fp2(F, E);                      /* F = E^2 */
    cy_fp2_sqr(E, F);

//    add_fp2(line[0], E, Q->X);          /* 3*A+X1 for line evaluation */
    cy_fp2_add(E, Q->X, line[0]);

//    sub_fp2(T->X, F, D);
    cy_fp2_sub(F, D, T->X);

//    sub_fp2(T->X, T->X, D);             /* X3 = F-2*D */
    cy_fp2_sub(T->X, D, T->X);

//    add_fp2(T->Z, Q->Y, Q->Z);
    cy_fp2_add(Q->Y, Q->Z, T->Z);

//    sqr_fp2(T->Z, T->Z);
    cy_fp2_sqr(T->Z, T->Z);

//    sub_fp2(T->Z, T->Z, B);
    cy_fp2_sub(T->Z, B, T->Z);

//    sub_fp2(T->Z, T->Z, ZZ);            /* Z3 = (Y1+Z1)^2-B-ZZ */
    cy_fp2_sqr(T->Z, ZZ, T->Z);

    mul_by_8_fp2(C, C);                 /* 8*C */
//    sub_fp2(T->Y, D, T->X);             /* D-X3 */
    cy_fp2_sub(D, T->X, T->Y);

//    mul_fp2(T->Y, T->Y, E);             /* E*(D-X3) */
    cy_fp2_mul(T->Y, E, T->Y);

//    sub_fp2(T->Y, T->Y, C);             /* Y3 = E*(D-X3)-8*C */
    cy_fp2_sub(T->Y, C, T->Y);

    /*
     * line evaluation
     */
//    sqr_fp2(line[0], line[0]);
    cy_fp2_sqr(line[0], line[0]);

//    sub_fp2(line[0], line[0], A);
    cy_fp2_sub(line[0], A, line[0]);

//    sub_fp2(line[0], line[0], F);       /* (3*A+X1)^2 - X1^2 - 9*A^2 */
    cy_fp2_sub(line[0], F, line[0]);

    lshift_fp2(B, B, 2);
//    sub_fp2(line[0], line[0], B);       /* 6*X1^3 - 4*Y1^2 */
    cy_fp2_sub(line[0], B, line[0]);

//    mul_fp2(line[1], E, ZZ);            /* 3*X1^2 * Z1^2 */
    cy_fp2_mul( E, ZZ, line[1]);

//    mul_fp2(line[2], T->Z, ZZ);         /* Z3 * Z1^2 */
    cy_fp2_mul(T->Z, ZZ, line[2]);

}

static void line_by_Px2(vec384fp6 line, const POINTonE1_affine *Px2)
{
//    mul_fp(line[1][0], line[1][0], Px2->X);   /* "b01" *= -2*P->X */
    mul_fp(line[1][1], line[1][1], Px2->X);

    mul_fp(line[2][0], line[2][0], Px2->Y);   /* "b11" *= 2*P->Y */
    mul_fp(line[2][1], line[2][1], Px2->Y);
}

static void start_dbl_n(vec384fp12 ret, POINTonE2 T[],
                                        const POINTonE1_affine Px2[], size_t n)
{
    size_t i;
    vec384fp6 line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    line_dbl(line, T+0, T+0);           line_by_Px2(line, Px2+0);
    vec_zero(ret, sizeof(vec384fp12));
    vec_copy(ret[0][0], line[0], 2*sizeof(vec384fp2));
    vec_copy(ret[1][1], line[2], sizeof(vec384fp2));

    for (i = 1; i < n; i++) {
        line_dbl(line, T+i, T+i);       line_by_Px2(line, Px2+i);
        mul_by_xy00z0_fp12(ret, ret, line);
    }
}

static void add_n_dbl_n(vec384fp12 ret, POINTonE2 T[],
                                        const POINTonE2_affine Q[],
                                        const POINTonE1_affine Px2[],
                                        size_t n, size_t k)
{
    size_t i;
    vec384fp6 line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    for (i = 0; i < n; i++) {
        line_add(line, T+i, T+i, Q+i);  line_by_Px2(line, Px2+i);
        mul_by_xy00z0_fp12(ret, ret, line);
    }
    while (k--) {
        sqr_fp12(ret, ret);
        for (i = 0; i < n; i++) {
            line_dbl(line, T+i, T+i);   line_by_Px2(line, Px2+i);
            mul_by_xy00z0_fp12(ret, ret, line);
        }
    }
}

static void miller_loop_n(vec384fp12 ret, const POINTonE2_affine Q[],
                                          const POINTonE1_affine P[], size_t n)
{
#if !defined(__STDC_VERSION__) || __STDC_VERSION__<199901
    POINTonE2 *T = alloca(n*sizeof(POINTonE2));
    POINTonE1_affine *Px2 = alloca(n*sizeof(POINTonE1_affine));
#else
    POINTonE2 T[n];
    POINTonE1_affine Px2[n];
#endif
    size_t i;

    if ((n == 1) && (vec_is_zero(&Q[0], sizeof(Q[0])) |
                     vec_is_zero(&P[0], sizeof(P[0]))) ) {
        /*
         * Special case of infinite aggregated signature, pair the additive
         * group's identity with the multiplicative group's identity.
         */
        vec_copy(ret, BLS12_381_Rx.p12, sizeof(vec384fp12));
        return;
    }

    for (i = 0; i < n; i++) {
        /* Move common expression from line evaluation to line_by_Px2.  */
        add_fp(Px2[i].X, P[i].X, P[i].X);
        neg_fp(Px2[i].X, Px2[i].X);
        add_fp(Px2[i].Y, P[i].Y, P[i].Y);

        vec_copy(T[i].X, Q[i].X, 2*sizeof(T[i].X));
        vec_copy(T[i].Z, BLS12_381_Rx.p2, sizeof(T[i].Z));
    }

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    start_dbl_n(ret, T, Px2, n);                /* 0x2                  */
    add_n_dbl_n(ret, T, Q, Px2, n, 2);          /* ..0xc                */
    add_n_dbl_n(ret, T, Q, Px2, n, 3);          /* ..0x68               */
    add_n_dbl_n(ret, T, Q, Px2, n, 9);          /* ..0xd200             */
    add_n_dbl_n(ret, T, Q, Px2, n, 32);         /* ..0xd20100000000     */
    add_n_dbl_n(ret, T, Q, Px2, n, 16);         /* ..0xd201000000010000 */
    conjugate_fp12(ret);                /* account for z being negative */
}

static void pre_add_n_dbl(vec384fp6 lines[], POINTonE2 *T,
                                             const POINTonE2_affine *Q,
                                             size_t n)
{
    line_add(lines++[0], T, T, Q);
    while (n--)
        line_dbl(lines++[0], T, T);
}

static void precompute_lines(vec384fp6 Qlines[68], const POINTonE2_affine *Q)
{
    POINTonE2 T[1];

    vec_copy(T->X, Q->X, 2*sizeof(T->X));
    vec_copy(T->Z, BLS12_381_Rx.p2, sizeof(T->Z));

    line_dbl(Qlines[0], T, T);                  /* 0x2                  */
    pre_add_n_dbl(&Qlines[1],  T, Q, 2);        /* ..0xc                */
    pre_add_n_dbl(&Qlines[4],  T, Q, 3);        /* ..0x68               */
    pre_add_n_dbl(&Qlines[8],  T, Q, 9);        /* ..0xd200             */
    pre_add_n_dbl(&Qlines[18], T, Q, 32);       /* ..0xd20100000000     */
    pre_add_n_dbl(&Qlines[51], T, Q, 16);       /* ..0xd201000000010000 */
}

static void post_line_by_Px2(vec384fp6 out, const vec384fp6 in,
                                            const POINTonE1_affine *Px2)
{
    vec_copy(out[0], in[0], sizeof(out[0]));

    mul_fp(out[1][0], in[1][0], Px2->X);        /* "b01" *= -2*P->X */
    mul_fp(out[1][1], in[1][1], Px2->X);

    mul_fp(out[2][0], in[2][0], Px2->Y);        /* "b11" *= 2*P->Y */
    mul_fp(out[2][1], in[2][1], Px2->Y);
}

static void post_add_n_dbl(vec384fp12 ret, const vec384fp6 lines[],
                           const POINTonE1_affine *Px2, size_t n)
{
    vec384fp6 line;

    post_line_by_Px2(line, lines++[0], Px2);
    mul_by_xy00z0_fp12(ret, ret, line);
    while (n--) {
        sqr_fp12(ret, ret);
        post_line_by_Px2(line, lines++[0], Px2);
        mul_by_xy00z0_fp12(ret, ret, line);
    }
}

static void miller_loop_lines(vec384fp12 ret, const vec384fp6 Qlines[68],
                                              const POINTonE1_affine *P)
{
    POINTonE1_affine Px2[1];
    vec384fp6 line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    /* Move common expression from line evaluation to line_by_Px2. */
    add_fp(Px2->X, P->X, P->X);
    neg_fp(Px2->X, Px2->X);
    add_fp(Px2->Y, P->Y, P->Y);

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    post_line_by_Px2(line, Qlines[0], Px2);     /* 0x2                  */
    vec_zero(ret, sizeof(vec384fp12));
    vec_copy(ret[0][0], line[0], 2*sizeof(vec384fp2));
    vec_copy(ret[1][1], line[2], sizeof(vec384fp2));
    post_add_n_dbl(ret, &Qlines[1],  Px2, 2);   /* ..0xc                */
    post_add_n_dbl(ret, &Qlines[4],  Px2, 3);   /* ..0x68               */
    post_add_n_dbl(ret, &Qlines[8],  Px2, 9);   /* ..0xd200             */
    post_add_n_dbl(ret, &Qlines[18], Px2, 32);  /* ..0xd20100000000     */
    post_add_n_dbl(ret, &Qlines[51], Px2, 16);  /* ..0xd201000000010000 */
    conjugate_fp12(ret);                /* account for z being negative */
}

static void mul_n_sqr(vec384fp12 ret, const vec384fp12 a, size_t n)
{
    mul_fp12(ret, ret, a);
    while (n--)
        cyclotomic_sqr_fp12(ret, ret);
}

static void raise_to_z_div_by_2(vec384fp12 ret, const vec384fp12 a)
{
    cyclotomic_sqr_fp12(ret, a);                /* 0x2                  */
    mul_n_sqr(ret, a, 2);                       /* ..0xc                */
    mul_n_sqr(ret, a, 3);                       /* ..0x68               */
    mul_n_sqr(ret, a, 9);                       /* ..0xd200             */
    mul_n_sqr(ret, a, 32);                      /* ..0xd20100000000     */
    mul_n_sqr(ret, a, 16-1);                    /* ..0x6900800000008000 */
    conjugate_fp12(ret);                /* account for z being negative */
}

#define raise_to_z(a, b) (raise_to_z_div_by_2(a, b), cyclotomic_sqr_fp12(a, a))

/*
 * Adaptation from <zkcrypto>/pairing/src/bls12_381/mod.rs
 */
static void final_exp(vec384fp12 ret, const vec384fp12 f)
{
    vec384fp12 y0, y1, y2, y3;

    vec_copy(y1, f, sizeof(y1));
    conjugate_fp12(y1);
    inverse_fp12(y2, f);
    mul_fp12(ret, y1, y2);
    frobenius_map_fp12(y2, ret, 2);
    mul_fp12(ret, ret, y2);

    cyclotomic_sqr_fp12(y0, ret);
    raise_to_z(y1, y0);
    raise_to_z_div_by_2(y2, y1);
    vec_copy(y3, ret, sizeof(y3));
    conjugate_fp12(y3);
    mul_fp12(y1, y1, y3);
    conjugate_fp12(y1);
    mul_fp12(y1, y1, y2);
    raise_to_z(y2, y1);
    raise_to_z(y3, y2);
    conjugate_fp12(y1);
    mul_fp12(y3, y3, y1);
    conjugate_fp12(y1);
    frobenius_map_fp12(y1, y1, 3);
    frobenius_map_fp12(y2, y2, 2);
    mul_fp12(y1, y1, y2);
    raise_to_z(y2, y3);
    mul_fp12(y2, y2, y0);
    mul_fp12(y2, y2, ret);
    mul_fp12(y1, y1, y2);
    frobenius_map_fp12(y2, y3, 1);
    mul_fp12(ret, y1, y2);
}

void blst_miller_loop(vec384fp12 ret, const POINTonE2_affine *Q,
                                      const POINTonE1_affine *P)
{   miller_loop_n(ret, Q ? Q : (const POINTonE2_affine *)&BLS12_381_G2,
                       P ? P : (const POINTonE1_affine *)&BLS12_381_G1, 1);
}

void blst_final_exp(vec384fp12 ret, const vec384fp12 f)
{   final_exp(ret, f);   }

void blst_precompute_lines(vec384fp6 Qlines[68], const POINTonE2_affine *Q)
{   precompute_lines(Qlines, Q);   }

void blst_miller_loop_lines(vec384fp12 ret, const vec384fp6 Qlines[68],
                                            const POINTonE1_affine *P)
{   miller_loop_lines(ret, Qlines, P);   }

static bool_t is_cyclotomic(const vec384fp12 f)
{
    vec384fp12 a, b;

    frobenius_map_fp12(a, f, 2);
    frobenius_map_fp12(b, a, 2);
    mul_fp12(b, b, f);

    return vec_is_equal(a, b, sizeof(a));
}

int blst_fp12_in_group(const vec384fp12 f)
{
    vec384fp12 a, b;

    if (vec_is_zero(f, sizeof(vec384fp12)) || !is_cyclotomic(f))
        return 0;

    frobenius_map_fp12(a, f, 1);
    raise_to_z(b, f);

    return (int)vec_is_equal(a, b, sizeof(a));
}
