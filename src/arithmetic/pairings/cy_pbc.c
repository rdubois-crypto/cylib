/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project */
/* License: This software is licensed under a dual BSD and GPL v2 license */
/* See LICENSE file at the root folder of the project. */
/*                                                                                */
/* 																			      */
/* FILE: cy_pairing.c */
/* 																			      */
/* 																			      */
/* DESCRIPTION: CY_LIB native bls12 pairings implementation   */
/* This is pairing from E(Fp)xE(Fp2)->FP12 over BLS12_381 implementation */
/**********************************************************************************/


/*
 * Copyright Supranational LLC
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <stddef.h>
#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_quad.h"
#include "cy_fp2.h"
#include "cy_fp2x3x2.h"
#include "cy_fp12.h"

/*
 * Line evaluations from  https://eprint.iacr.org/2010/354.pdf
 * with a twist moving common expression to line_by_Px2.
 */
#define _COORD_FP2_HEADER \
cy_fp2_t*	X;\
cy_fp2_t*	Y;\
cy_fp2_t*	Z;
//  ecfp2_ctx_t *ctx;

struct cy_ecfp2_s {
	_COORD_FP2_HEADER /* the coordonates are defined by the EC_UNIT used*/
  boolean_t is_normalized ;

  uint8_t *index;

};

typedef struct cy_ecfp2_s cy_ecfp2point_t;

#define _COORD_HEADER \
cy_fp_t*	X;\
cy_fp_t*	Y;\
cy_fp_t*	Z;

struct cy_ec_s {
 _COORD_HEADER /* the coordonates are defined by the EC_UNIT used*/
  boolean_t is_normalized ;
  uint8_t *index;

};


typedef struct cy_ec_s cy_ecpoint_t;

cy_error_t cy_line_add( const cy_ecfp2point_t *R,
                                                   const cy_ecfp2point_t *Q, cy_fp6_t line, cy_ecfp2point_t *T)
{
	cy_error_t error;
    /*vec384x Z1Z1, U2, S2, H, HH, I, J, V;*/

	cy_fp2_t Z1Z1, U2, S2, H, HH, I, J, V;
	fp2_ctx_t *ctx_fp2= line[0].ctx_quad;

	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &Z1Z1 ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &U2 ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &S2 ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &H ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &HH ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &I ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &J ));
	 CY_CHECK(cy_fp2_alloc(ctx_fp2, &V ));

	/* todo: allocate*/

	#define r line[1]

    /*
     * https://hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-madd-2007-bl
     * with XYZ3 being |T|, XYZ1 - |R|, XY2 - |Q|, i.e. Q is affine
     */
//    sqr_fp2(Z1Z1, R->Z);                /* Z1Z1 = Z1^2 */
	cy_fp2_sqr(R->Z, &Z1Z1);
//    mul_fp2(U2, Q->X, Z1Z1);            /* U2 = X2*Z1Z1 */
	cy_fp2_mul( &Z1Z1, Q->X, &Z1Z1);

//    mul_fp2(S2, Q->Y, R->Z);
	cy_fp2_mul(  Q->Y, R->Z, &S2);

//    mul_fp2(S2, S2, Z1Z1);              /* S2 = Y2*Z1*Z1Z1 */
	cy_fp2_mul(  &S2, &Z1Z1, &S2);

//    sub_fp2(H, U2, R->X);               /* H = U2-X1 */
	cy_fp2_sub(  &U2, R->X, &H);

//    sqr_fp2(HH, H);                     /* HH = H^2 */
	cy_fp2_sqr(  &H, &HH);


//    add_fp2(I, HH, HH);
	cy_fp2_add(  &HH, &HH, &I);

//    add_fp2(I, I, I);                   /* I = 4*HH */
	cy_fp2_add(  &I, &I, &I);

//    mul_fp2(J, H, I);                   /* J = H*I */
	cy_fp2_mul(  &H, &I, &J);

//    sub_fp2(r, S2, R->Y);
	cy_fp2_sub(&S2, R->Y, &r);

//    add_fp2(r, r, r);                   /* r = 2*(S2-Y1) */
	cy_fp2_add(&r,&r,&r);

//    mul_fp2(V, R->X, I);                /* V = X1*I */
	cy_fp2_mul(&I,R->X,&V);

//    sqr_fp2(T->X, r);
	cy_fp2_sqr(&r,T->X);

//    sub_fp2(T->X, T->X, J);
	cy_fp2_sub( T->X, &J, T->X);

//    sub_fp2(T->X, T->X, V);
	cy_fp2_sub(T->X, &V, T->X);
//    sub_fp2(T->X, T->X, V);             /* X3 = r^2-J-2*V */
	cy_fp2_sub(T->X, &V, T->X);
//TODO
//    mul_fp2(J, J, R->Y);
	cy_fp2_mul(&J, R->Y, &J);

//    sub_fp2(T->Y, V, T->X);
	cy_fp2_sub(&V, T->X, T->Y);

//    mul_fp2(T->Y, T->Y, r);
	cy_fp2_mul(T->Y, &r, T->Y);

//    sub_fp2(T->Y, T->Y, J);
	cy_fp2_sub(T->Y, &J, T->Y);
//    sub_fp2(T->Y, T->Y, J);             /* Y3 = r*(V-X3)-2*Y1*J */
	cy_fp2_sub(T->Y, &J, T->Y);

//    add_fp2(T->Z, R->Z, H);
	cy_fp2_add(R->Z, &H, T->Z);

    //sqr_fp2(T->Z, T->Z);
	cy_fp2_sqr(T->Z, T->Z);

    //sub_fp2(T->Z, T->Z, Z1Z1);
	cy_fp2_sub(T->Z,& Z1Z1, T->Z);

//    sub_fp2(T->Z, T->Z, HH);            /* Z3 = (Z1+H)^2-Z1Z1-HH */
	cy_fp2_sub(T->Z, &HH, T->Z);

    /*
     * line evaluation
     */
//    mul_fp2(I, r, Q->X);
	cy_fp2_mul(&r, Q->X,&I);

//    mul_fp2(J, Q->Y, T->Z);
	cy_fp2_mul(Q->Y, T->Z, &J);

//    sub_fp2(I, I, J);
	cy_fp2_mul(&I, &J, &I);

//    add_fp2(line[0], I, I);          /* 2*(r*X2 - Y2*Z3) */
	cy_fp2_add(&I, &I, &line[0]);

	//TODO
# undef r

    cy_fp2_copy( T->Z, &line[2]);

    CY_CHECK(cy_fp2_free( &Z1Z1 ));
   	 CY_CHECK(cy_fp2_free( &U2 ));
   	 CY_CHECK(cy_fp2_free( &S2 ));
   	 CY_CHECK(cy_fp2_free( &H ));
   	 CY_CHECK(cy_fp2_free( &HH ));
   	 CY_CHECK(cy_fp2_free( &I ));
   	 CY_CHECK(cy_fp2_free( &J ));
   	 CY_CHECK(cy_fp2_free( &V ));

    end:
	return error;
}

/* todo*/
#define mul_by_8_fp2(a,b) return CY_NOT_IMPLEMENTED
#define mul_by_3_fp2(a,b) return CY_NOT_IMPLEMENTED
#define mul_by_8_fp3(a,b) return CY_NOT_IMPLEMENTED
#define lshift_fp2(a,b,c) return CY_NOT_IMPLEMENTED

cy_error_t cy_line_dbl(cy_ecfp2point_t *T, const cy_ecfp2point_t *Q, cy_fp6_t line)
{
	cy_error_t error;
	cy_fp2_t ZZ, A, B, C, D, E, F;

    fp2_ctx_t *ctx_fp2= line[0].ctx_quad;

     CY_CHECK(cy_fp2_alloc(ctx_fp2, &A ));
     CY_CHECK(cy_fp2_alloc(ctx_fp2, &B ));
     CY_CHECK(cy_fp2_alloc(ctx_fp2, &C ));
     CY_CHECK(cy_fp2_alloc(ctx_fp2, &D ));
     CY_CHECK(cy_fp2_alloc(ctx_fp2, &E ));
     CY_CHECK(cy_fp2_alloc(ctx_fp2, &F ));



    /*
     * https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#doubling-dbl-2009-alnr
     */
//    sqr_fp2(A, Q->X);                   /* A = X1^2 */
    cy_fp2_sqr(Q->X, &A);

//    sqr_fp2(B, Q->Y);                   /* B = Y1^2 */
    cy_fp2_sqr(Q->Y, &B);

//    sqr_fp2(ZZ, Q->Z);                  /* ZZ = Z1^2 */
    cy_fp2_sqr(Q->Z, &ZZ);

//    sqr_fp2(C, B);                      /* C = B^2 */
    cy_fp2_sqr(&B,&C);

//    add_fp2(D, Q->X, B);                /* X1+B */
    cy_fp2_add(Q->X, &B, &D);

 //   sqr_fp2(D, D);                      /* (X1+B)^2 */
    cy_fp2_sqr(&D, &D);

 //   sub_fp2(D, D, A);                   /* (X1+B)^2-A */
    cy_fp2_sub(&D, &A, &D);

//    sub_fp2(D, D, C);                   /* (X1+B)^2-A-C */
    cy_fp2_sub(&D, &C, &D);

//    add_fp2(D, D, D);                   /* D = 2*((X1+B)^2-A-C) */
    cy_fp2_add(&D, &D, &D);

    mul_by_3_fp2(&E, &A);                 /* E = 3*A */
//    sqr_fp2(F, E);                      /* F = E^2 */
    cy_fp2_sqr(&E, &F);

//    add_fp2(line[0], E, Q->X);          /* 3*A+X1 for line evaluation */
    cy_fp2_add(&E, Q->X, &line[0]);

//    sub_fp2(T->X, F, D);
    cy_fp2_sub(&F, &D, T->X);

//    sub_fp2(T->X, T->X, D);             /* X3 = F-2*D */
    cy_fp2_sub(T->X, &D, T->X);

//    add_fp2(T->Z, Q->Y, Q->Z);
    cy_fp2_add(Q->Y, Q->Z, T->Z);

//    sqr_fp2(T->Z, T->Z);
    cy_fp2_sqr(T->Z, T->Z);

//    sub_fp2(T->Z, T->Z, B);
    cy_fp2_sub(T->Z, &B, T->Z);

//    sub_fp2(T->Z, T->Z, ZZ);            /* Z3 = (Y1+Z1)^2-B-ZZ */
    cy_fp2_sub(T->Z, &ZZ, T->Z);

    mul_by_8_fp2(C, C);                 /* 8*C */
//    sub_fp2(T->Y, D, T->X);             /* D-X3 */
    cy_fp2_sub(&D, T->X, T->Y);

//    mul_fp2(T->Y, T->Y, E);             /* E*(D-X3) */
    cy_fp2_mul(T->Y, &E, T->Y);

//    sub_fp2(T->Y, T->Y, C);             /* Y3 = E*(D-X3)-8*C */
    cy_fp2_sub(T->Y, &C, T->Y);

    /*
     * line evaluation
     */
//    sqr_fp2(line[0], line[0]);
    cy_fp2_sqr(&line[0], &line[0]);

//    sub_fp2(line[0], line[0], A);
    cy_fp2_sub(&line[0], &A, &line[0]);

//    sub_fp2(line[0], line[0], F);       /* (3*A+X1)^2 - X1^2 - 9*A^2 */
    cy_fp2_sub(&line[0], &F, &line[0]);

    lshift_fp2(&B, &B, 2);
//    sub_fp2(line[0], line[0], B);       /* 6*X1^3 - 4*Y1^2 */
    cy_fp2_sub(&line[0], &B, &line[0]);

//    mul_fp2(line[1], E, ZZ);            /* 3*X1^2 * Z1^2 */
    cy_fp2_mul( &E, &ZZ, &line[1]);

//    mul_fp2(line[2], T->Z, ZZ);         /* Z3 * Z1^2 */
    cy_fp2_mul(T->Z, &ZZ, &line[2]);

    CY_CHECK(cy_fp2_free( &A ));
    CY_CHECK(cy_fp2_free( &B ));
    CY_CHECK(cy_fp2_free( &C ));
    CY_CHECK(cy_fp2_free( &D ));
    CY_CHECK(cy_fp2_free( &E ));
    CY_CHECK(cy_fp2_free( &F ));

    end:
	return error;
}



cy_error_t cy_line_by_Px2(const cy_ecpoint_t *Px2, cy_fp6_t line)
{
	cy_error_t error;
	error=CY_OK;

//    mul_fp(line[1][0], line[1][0], Px2->X);   /* "b01" *= -2*P->X */
	cy_fp_mul( line[1].x, Px2->X, line[1].x);

 //   mul_fp(line[1][1], line[1][1], Px2->X);
	cy_fp_mul( line[1].y, Px2->X, line[1].y);

//    mul_fp(line[2][0], line[2][0], Px2->Y);   /* "b11" *= 2*P->Y */
	cy_fp_mul( line[2].x, Px2->Y, line[2].y);

//    mul_fp(line[2][1], line[2][1], Px2->Y);
    cy_fp_mul( line[2].y, Px2->Y, line[2].y);

    return error;

}

#define cy_mul_by_xy00z0_fp12(a,b,out) return CY_NOT_IMPLEMENTED

 cy_error_t cy_start_dbl_n( cy_ecfp2point_t T[],
                                        const cy_ecpoint_t Px2[], size_t n, cy_fp12_t *ret)
{
    size_t i;
    cy_error_t error;
    cy_fp6_t line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    cy_line_dbl(T+0, T+0, line);           cy_line_by_Px2(Px2+0, line);
    CY_CHECK(cy_fp12_set_zero(ret));
    cy_fp2_copy(&line[0], &(*ret)[0][0] );
    cy_fp2_copy( &line[2], &(*ret)[1][1]);

    for (i = 1; i < n; i++) {
        cy_line_dbl( T+i, T+i, line);       cy_line_by_Px2( Px2+i, line);
        cy_mul_by_xy00z0_fp12(ret, ret, line);
    }

    end:
		return error;
}

 cy_error_t cy_add_n_dbl_n(       const cy_ecfp2point_t Q[],
                                        const cy_ecpoint_t Px2[],
                                        size_t n, size_t k, cy_fp12_t *ret, cy_ecfp2point_t T[])
{
	cy_error_t error;
    size_t i;
    cy_fp6_t line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    for (i = 0; i < n; i++) {
        CY_CHECK(cy_line_add(  T+i, Q+i, line, T+i));
        cy_line_by_Px2( Px2+i, line);
        cy_mul_by_xy00z0_fp12(ret, ret, line);
    }
    while (k--) {
        //sqr_fp12(ret, ret);
    	cy_fp12_sqr(ret, ret);

        for (i = 0; i < n; i++) {
            cy_line_dbl(T+i, T+i, line);   cy_line_by_Px2( Px2+i, line);
            cy_mul_by_xy00z0_fp12(ret, ret, line);
        }
    }

    end:
		return error;
}

#define cy_ecfp2_iszero(a,b) return CY_NOT_IMPLEMENTED
#define cy_ec_iszero(a,b) return CY_NOT_IMPLEMENTED
#define cy_fp2_set_one(a) return CY_NOT_IMPLEMENTED


cy_error_t miller_loop_n( const cy_ecfp2point_t Q[],
                                          const cy_ecpoint_t P[], size_t n, cy_fp12_t *ret)
{
	cy_error_t error;
//    POINTonE2 T[n];
//    POINTonE1_affine Px2[n];

	cy_ecfp2point_t *T;
	cy_ecpoint_t *Px2;
	/* todo: allocation */
	int iszero_c1, iszero_c2;
	size_t i;

	cy_ecfp2_iszero(Q[0].X, &iszero_c1);
	cy_ec_iszero(P[0].X, &iszero_c2);



    if ((n == 1) && (iszero_c1 |
    		iszero_c2) ) {
        /*
         * Special case of infinite aggregated signature, pair the additive
         * group's identity with the multiplicative group's identity.
         */

    	cy_fp2_set_one(ret);
    	error=CY_OK;
        goto end;
    }

    for (i = 0; i < n; i++) {
        /* Move common expression from line evaluation to line_by_Px2.  */
        cy_fp_add(P[i].X, P[i].X, Px2[i].X);
        cy_fp_neg(Px2[i].X, Px2[i].X);
        cy_fp_add( P[i].Y, P[i].Y, Px2[i].Y);

        cy_fp2_copy( Q[i].X,(T[i]).X);
        /* todo setone cy_fp2_copy( BLS12_381_Rx.p2, T[i].Z);*/
    }

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    cy_start_dbl_n( T, Px2, n, ret);                /* 0x2                  */
    cy_add_n_dbl_n(  Q, Px2, n, 2, ret, T);          /* ..0xc                */
    cy_add_n_dbl_n(  Q, Px2, n, 3, ret, T);          /* ..0x68               */
    cy_add_n_dbl_n(  Q, Px2, n, 9, ret, T);          /* ..0xd200             */
    cy_add_n_dbl_n(  Q, Px2, n, 32, ret, T);         /* ..0xd20100000000     */
    cy_add_n_dbl_n(  Q, Px2, n, 16, ret, T);         /* ..0xd201000000010000 */
    cy_fp12_conjugate(ret);                /* account for z being negative */

    end:
  		return error;
}


/* todo: vérifier affine/proj*/
static cy_error_t cy_pre_add_n_dbl(                                             const cy_ecfp2point_t *Q,
                                             size_t n, cy_fp6_t lines[], cy_ecfp2point_t *T)
{
	cy_error_t error;
    CY_CHECK(cy_line_add( T, Q, lines++[0], T));
    while (n--)
        cy_line_dbl( T, T, lines++[0]);

    end:
		return error;
}

#define cy_fp2_set_one(a) return CY_NOT_IMPLEMENTED

cy_error_t cy_precompute_lines( const cy_ecfp2point_t *Q, cy_fp6_t Qlines[68])
{
	cy_ecfp2point_t T;/* todo allocation*/
	fp2_ctx_t *ctx_fp2= Qlines[0][0].ctx_quad;

	T.X=NULL;

	cy_fp2_alloc(ctx_fp2, T.X);
	cy_fp2_alloc(ctx_fp2, T.Z);

	//cy_fp2_alloc(T->X);

	cy_error_t error;

    CY_CHECK(cy_fp2_copy( Q->X, T.X));
    cy_fp2_set_one( & T->Z); /* normalized T*/

    cy_line_dbl( &T, &T, Qlines[0]);                  /* 0x2                  */
    cy_pre_add_n_dbl( Q, 2, &Qlines[1],  &T);        /* ..0xc                */
    cy_pre_add_n_dbl( Q, 3, &Qlines[4],  &T);        /* ..0x68               */
    cy_pre_add_n_dbl( Q, 9, &Qlines[8],  &T);        /* ..0xd200             */
    cy_pre_add_n_dbl( Q, 32, &Qlines[18], &T);       /* ..0xd20100000000     */
    cy_pre_add_n_dbl(Q, 16, &Qlines[51], &T);       /* ..0xd201000000010000 */

    cy_fp2_free( T.X);
    cy_fp2_free( T.Z);

    end:
		return error;
}


static cy_error_t cy_post_line_by_Px2( const cy_fp6_t in,
                                            const cy_ecpoint_t *Px2, cy_fp6_t out)
{
	cy_error_t error;


	CY_CHECK(cy_fp2_copy(&in[0], &out[0]));

    cy_fp_mul( in[1].x, Px2->X, out[1].x);        /* "b01" *= -2*P->X */
    cy_fp_mul( in[1].y, Px2->X, out[1].y);

    cy_fp_mul( in[2].x, Px2->Y, out[2].x);        /* "b11" *= 2*P->Y */
    cy_fp_mul( in[2].y, Px2->Y, out[2].y);

    end:
  		return error;
}

 cy_error_t cy_post_add_n_dbl( const cy_fp6_t lines[],
                           const cy_ecpoint_t *Px2, size_t n, cy_fp12_t *ret)
{
	cy_fp6_t line;
	fp2_ctx_t *ctx_fp2= (*ret)[0][0].ctx_quad;
	cy_fp6_alloc(ctx_fp2, &line);

	//todo : allocation
	cy_error_t error;

    CY_CHECK(cy_post_line_by_Px2( lines++[0], Px2, line));
    cy_mul_by_xy00z0_fp12(ret, line, ret);
    while (n--) {
        cy_fp12_sqr(ret, ret);
        cy_post_line_by_Px2( lines++[0], Px2, line);
        cy_mul_by_xy00z0_fp12(ret, line,  ret);
    }
    cy_fp6_alloc(ctx_fp2, &line);

    end:
  		return error;
}

 cy_error_t miller_loop_lines(const cy_fp6_t Qlines[68],
                                              const cy_ecpoint_t *P, cy_fp12_t *ret)
{
	cy_ecpoint_t Px2;
	cy_error_t error=CY_KO ;
	/*todo: allocation*/
	cy_fp_ctx_t *ctx_fp= (*ret)[0][0].ctx_fp;
	size_t t8_p=ctx_fp->t8_modular;
	Px2.X=NULL;

	cy_fp_alloc(ctx_fp,t8_p,  Px2.X);
	cy_fp_alloc(ctx_fp,t8_p,  Px2.Y);

    cy_fp6_t line; /* it's not actual fp6, but 3 packed fp2, "xy00z0"  */

    /* Move common expression from line evaluation to line_by_Px2. */
    cy_fp_add( P->X, P->X, Px2.X);
    cy_fp_neg(Px2.X, Px2.X);
    cy_fp_add( P->Y, P->Y, Px2.Y);

    /* first step is ret = 1^2*line, which is replaced with ret = line  */
    cy_post_line_by_Px2( Qlines[0], &Px2, line);     /* 0x2                  */
    cy_fp12_set_zero(ret);
    cy_fp2_copy(&line[0], &(*ret)[0][0]);
    cy_fp2_copy( &((*ret)[1][1]), &line[2]);
    cy_post_add_n_dbl( &Qlines[1],  &Px2, 2, ret);   /* ..0xc                */
    cy_post_add_n_dbl( &Qlines[4],  &Px2, 3, ret);   /* ..0x68               */
    cy_post_add_n_dbl( &Qlines[8],  &Px2, 9, ret);   /* ..0xd200             */
    cy_post_add_n_dbl( &Qlines[18], &Px2, 32, ret);  /* ..0xd20100000000     */
    cy_post_add_n_dbl( &Qlines[51], &Px2, 16, ret);  /* ..0xd201000000010000 */
    cy_fp6_neg((*ret)[1], (*ret)[1]);                /* account for z being negative */

    return error;
}

static void cy_mul_n_sqr( const cy_fp12_t *a,  size_t n, cy_fp12_t *ret)
{
    cy_fp12_mul( ret, a, ret);
    while (n--)
    	cy_fp12_cyclotomic_sqr(ret, ret);
}

cy_error_t cy_raise_to_z_div_by_2( const cy_fp12_t *a, cy_fp12_t *ret)
{
	cy_error_t error=CY_KO;

	CY_CHECK(cy_fp12_cyclotomic_sqr( a, ret));                /* 0x2                  */
	cy_mul_n_sqr( a, 2, ret);                       /* ..0xc                */
	cy_mul_n_sqr( a, 3, ret);                       /* ..0xc                */
	cy_mul_n_sqr( a, 9, ret);                       /* ..0xc                */
	cy_mul_n_sqr( a, 32, ret);                       /* ..0xc                */
	cy_mul_n_sqr( a, 16-1, ret);                       /* ..0xc                */
	cy_fp12_conjugate(ret);                /* account for z being negative */

	end:
		return error;
}

#define cy_raise_to_z(a, b) (cy_raise_to_z_div_by_2(a, b), cy_fp12_cyclotomic_sqr(b, b))

#define cy_fp12_frobenius_map(a,b,c) return CY_NOT_IMPLEMENTED ;

/*
 * Adaptation from <zkcrypto>/pairing/src/bls12_381/mod.rs
 */
 cy_error_t final_exp( const cy_fp12_t *f, cy_fp12_t *ret)
{
	cy_error_t error;
    cy_fp12_t y0, y1, y2, y3;
    /* allocations*/
   	fp2_ctx_t *ctx_fp2= ((*ret)[0][0]).ctx_quad;

   	CY_CHECK(cy_fp2x6_alloc(ctx_fp2, &y0));
   	cy_fp2x6_alloc(ctx_fp2, &y1);
   	cy_fp2x6_alloc(ctx_fp2, &y2);
   	cy_fp2x6_alloc(ctx_fp2, &y3);

    cy_fp12_copy( f, &y1);
    //conjugate_fp12(y1);
    cy_fp12_conjugate(&y1);
    // inverse_fp12(y2, f);
    cy_fp12_inv(f, &y2);
    // mul_fp12(ret, y1, y2);
    cy_fp12_mul(&y1, &y2, ret);
    // frobenius_map_fp12(y2, ret, 2);
    cy_fp12_frobenius_map( ret, 2, &y2);
    // mul_fp12(ret, ret, y2);
    cy_fp12_mul(ret,& y2, ret);
   // cyclotomic_sqr_fp12(y0, ret);
    cy_fp12_cyclotomic_sqr( ret, &y0);
    //raise_to_z(y1, y0);
    cy_raise_to_z( &y0, &y1);
    //raise_to_z_div_by_2(y2, y1);
    cy_raise_to_z_div_by_2( &y1, &y2);

    cy_fp12_copy( ret, &(y3));
    cy_fp12_conjugate(&y3);

    //mul_fp12(y1, y1, y3);
    cy_fp12_mul( &y1, &y3, &y1);

    //conjugate_fp12(y1);
    cy_fp12_conjugate(&y1);

    //mul_fp12(y1, y1, y2);
    cy_fp12_mul(&y1, &y2, &y1);

//    raise_to_z(y2, y1);
    cy_raise_to_z(& y1, &y2);

//    raise_to_z(y3, y2);
    cy_raise_to_z(& y2, &y3);

    //conjugate_fp12(y1);
    cy_fp12_conjugate(&y1);

//    mul_fp12(y3, y3, y1);
    cy_fp12_mul(&y1, &y2, &y1);

//    conjugate_fp12(y1);
    cy_fp12_conjugate(&y1);

   // frobenius_map_fp12(y1, y1, 3);
    cy_fp12_frobenius_map(&y1, 3, &y1);

//    frobenius_map_fp12(y2, y2, 2);
    cy_fp12_frobenius_map(&y2, 2, &y2);

//    mul_fp12(y1, y1, y2);
    cy_fp12_frobenius_map(&y1, &y2, &y1);

//    raise_to_z(y2, y3);
    cy_raise_to_z(& y3, &y2);

//    mul_fp12(y2, y2, y0);
    cy_fp12_mul(&y2, &y0, &y2);

//    mul_fp12(y2, y2, ret);
    cy_fp12_mul(&y2, ret, &y2);

//    mul_fp12(y1, y1, y2);
    cy_fp12_mul(&y1, &y2, &y1);

//    frobenius_map_fp12(y2, y3, 1);
    cy_fp12_frobenius_map(& y3, 1, &y2);
    //mul_fp12(ret, y1, y2);
    cy_fp12_mul(&y1, &y2, ret);

   	cy_fp12_free( &y0);
   	cy_fp12_free( &y1);
   	cy_fp12_free( &y2);
   	cy_fp12_free( &y3);

   	end:
	return error;
}


static cy_error_t cy_is_cyclotomic(const cy_fp12_t *f, boolean_t *res)
{
	cy_error_t error=CY_KO;
    cy_fp12_t a, b;
    cy_fp12_frobenius_map( f, 2, &a);
    cy_fp12_frobenius_map( &a, 2, &b);
    cy_fp12_mul(&b, f, &b);

    cy_fp12_iseq(&a, &b, res);

    return error;
}

#define cy_fp12_iszero(a,b) return CY_NOT_IMPLEMENTED

cy_error_t cy_fp12_in_group(const cy_fp12_t *f, int *res)
{
	cy_error_t error;
	cy_fp12_t a, b;

	CY_CHECK(cy_is_cyclotomic(f, res));
	if(*res==CY_FALSE)
		{
			goto end;
		}
	cy_fp12_iszero(f, res);
	if(*res==CY_FALSE)
	{
		goto end;
	}

    cy_fp12_frobenius_map( f, 1,&a);
    cy_raise_to_z( f, &b);
    cy_fp12_iseq(&a, &b, res);

    end:
		return error;

}
