/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_fp12.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: cy_lib NATIVE dodecaic field extensions for cy_fp12.h implementation using */
/* FP12=((FP2)^3)^2, implementation adapted from blst library  */
/* Fp modulus must have -1 as a square */
/**********************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "cy_configuration.h"
#include "cy_quad.h"
#include "cy_fp2.h"
#include "cy_fp2x3x2.h"
#include "cy_fp12.h"

typedef cy_fp2_t cy_fp4_t[2];


/* TODO: revise initializer*/
cy_error_t cy_fp2x3x2_init(fp12_ctx_t *ps_ctx_fp12, uint8_t *pu8_Mem,
                       const size_t t8_Memory, const int argc,
                       const uint8_t *argv[])
{
  cy_error_t error = CY_KO;

  if (argc != 2) {
    return CY_KO;
  }
  /*control memory limit*/
  if (t8_Memory > _MAX_MEMORY) {
    error = CY_KO;
    goto end;
  }


  strcpy(ps_ctx_fp12->libname, CY_FP12_LIBNAME);

  if(ps_ctx_fp12->ctx_quad.is_initialized!=CY_LIB_INITIALIZED)
  {
	  CY_CHECK(cy_fp2_init(&ps_ctx_fp12->ctx_quad, pu8_Mem, t8_Memory, argc, argv));
  }
  else
  {/* if the ctx is already initialized*/
	  error=CY_OK;
  }

end:
  return error;
}


cy_error_t cy_fp2x3x2_uninit(fp12_ctx_t *ps_ctx_fp12, uint8_t *pu8_Mem,
                       const size_t t8_Memory){
	cy_error_t error = CY_KO;


	CY_CHECK(cy_fp2_uninit(&ps_ctx_fp12->ctx_quad, pu8_Mem, t8_Memory));

	ps_ctx_fp12->ctx_quad.is_initialized=CY_LIB_UNINITIALIZED;

	end:
	  return error;
}

/* multiplication by u+1, assuming -1 is square mod p */
static inline cy_error_t cy_mul_by_u_plus_1_fp2( const cy_fp2_t *a, cy_fp2_t *ret)
{
  cy_error_t error = CY_KO;

  CY_CHECK(cy_fp_add(a->x, a->y, ret->y));
  CY_CHECK(cy_fp_sub(a->x, a->y, ret->x));

  end:
    return error;
}


/*
 * Fp6 extension
 */
static inline cy_error_t cy_fp6_mul( const cy_fp6_t a, const cy_fp6_t b, cy_fp6_t ret)
{
 cy_error_t error;
//  vec384x t0, t1, t2, t3, t4, t5;
 cy_fp2_t t0, t1, t2, t3, t4, t5;
 fp2_ctx_t *ctx_fp2=a[0].ctx_quad;

 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t0 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t1 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t2 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t3 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t4 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &t5 ));

 // mul_fp2(t0, a[0], b[0]);
  cy_fp2_mul(&(a[0]), &(b[0]), &t0);

//  mul_fp2(t1, a[1], b[1]);
  cy_fp2_mul(&a[1], &b[1], &t1);

  //mul_fp2(t2, a[2], b[2]);
  cy_fp2_mul(&a[2], &b[2], &t2);

  /* ret[0] = ((a1 + a2)*(b1 + b2) - a1*b1 - a2*b2)*(u+1) + a0*b0
            = (a1*b2 + a2*b1)*(u+1) + a0*b0 */
  //add_fp2(t4, a[1], a[2]);
  cy_fp2_add(&a[1], &a[2], &t4);

//  add_fp2(t5, b[1], b[2]);
  cy_fp2_add(&b[1], &b[2], &t5);

//  mul_fp2(t3, t4, t5);
  cy_fp2_mul(&t4, &t5, &t3);

 // sub_fp2(t3, t3, t1);
  cy_fp2_sub(&t3, &t1, &t3);

  //sub_fp2(t3, t3, t2);
  cy_fp2_sub(&t3, &t2, &t3);

  cy_mul_by_u_plus_1_fp2(&t3, &t3);
  /* add_fp2(ret[0], t3, t0); considering possible aliasing... */
  cy_fp2_add(&t0, &t3, &ret[0]);

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1 + a2*b2*(u+1)
            = a0*b1 + a1*b0 + a2*b2*(u+1) */
 // add_fp2(t4, a[0], a[1]);
  cy_fp2_add(&a[0], &a[1], &t4);

  //add_fp2(t5, b[0], b[1]);
  cy_fp2_add(&b[0], &b[1], &t5);

  //mul_fp2(ret[1], t4, t5);
  cy_fp2_mul(&t4, &t5, &ret[1]);

//  sub_fp2(ret[1], ret[1], t0);
  cy_fp2_sub(&ret[1], &t0, &ret[1]);

//  sub_fp2(ret[1], ret[1], t1);
  cy_fp2_sub(&ret[1], &t1, &ret[1]);

  //mul_by_u_plus_1_fp2(t4, t2);
  cy_mul_by_u_plus_1_fp2(&t2, &t4);

//  add_fp2(ret[1], ret[1], t4);
  cy_fp2_add(&ret[1], &t4, &ret[1]);

  /* ret[2] = (a0 + a2)*(b0 + b2) - a0*b0 - a2*b2 + a1*b1
            = a0*b2 + a2*b0 + a1*b1 */
//  add_fp2(t4, a[0], a[2]);
  cy_fp2_add(&a[0], &a[0], &t4);

//  add_fp2(t5, b[0], b[2]);
  cy_fp2_add(&b[0], &b[2], &t5);

//  mul_fp2(ret[2], t4, t5);
  cy_fp2_mul(&t4, &t5, &ret[2]);

//  sub_fp2(ret[2], ret[2], t0);
  cy_fp2_sub(&ret[2], &t0, &ret[1]);

//  sub_fp2(ret[2], ret[2], t2);
  cy_fp2_sub(&ret[2], &t2, &ret[2]);

//  add_fp2(ret[2], ret[2], t1);
  cy_fp2_add(&t1, &ret[2], &ret[2]);

//  add_fp2(ret[0], t3, t0); /* ... moved from above */
  cy_fp2_add(&t0, &t3, &ret[0]);

  cy_fp2_free( &t0 );
  cy_fp2_free( &t1 );
  cy_fp2_free( &t2 );
  cy_fp2_free( &t3 );
  cy_fp2_free( &t4 );
  cy_fp2_free( &t5 );

  end:
   return error;
}

cy_error_t cy_conjugate_fp12(cy_fp12_t *out)
{
	cy_error_t error;

	CY_CHECK(cy_fp6_neg((*out)[1], (*out)[1]));

	  end:
	   return error;
}

static cy_error_t cy_fp6_sqr( const cy_fp6_t a, cy_fp6_t ret)
{
//  vec384x s0, m01, m12, s2;
 cy_error_t error;
 cy_fp2_t s0, m01, m12, s2;
 fp2_ctx_t *ctx_fp2=a[0].ctx_quad;

 CY_CHECK(cy_fp2_alloc(ctx_fp2, &s0 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &m01 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &m12 ));
 CY_CHECK(cy_fp2_alloc(ctx_fp2, &s2 ));


 // sqr_fp2(s0, a[0]);
 cy_fp2_sqr(&a[0], &s0);

//  mul_fp2(m01, a[0], a[1]);
 cy_fp2_mul(&a[0], &a[1],&m01);

//  add_fp2(m01, m01, m01);
 cy_fp2_add(&m01, &m01, &m01);

//  mul_fp2(m12, a[1], a[2]);
 cy_fp2_mul(&a[2], &a[1],&m12);

//  add_fp2(m12, m12, m12);
 cy_fp2_add(&m12, &m12, &m12);

//  sqr_fp2(s2, a[2]);
 cy_fp2_sqr(&a[2], &s2);

  /* ret[2] = (a0 + a1 + a2)^2 - a0^2 - a2^2 - 2*(a0*a1) - 2*(a1*a2)
            = a1^2 + 2*(a0*a2) */
//  add_fp2(ret[2], a[2], a[1]);
 cy_fp2_add(&a[2], &a[1], &ret[2]);

//  add_fp2(ret[2], ret[2], a[0]);
 cy_fp2_add(&ret[2], &a[0], &ret[2]);

//  sqr_fp2(ret[2], ret[2]);
 cy_fp2_sqr(&ret[2], &ret[2]);

//  sub_fp2(ret[2], ret[2], s0);
 cy_fp2_sub(&ret[2], &s0, &ret[2]);

//  sub_fp2(ret[2], ret[2], s2);
 cy_fp2_sub(&ret[2], &s2, &ret[2]);

//  sub_fp2(ret[2], ret[2], m01);
 cy_fp2_sub(&ret[2], &m01, &ret[2]);

//  sub_fp2(ret[2], ret[2], m12);
 cy_fp2_sub(&ret[2], &m12, &ret[2]);

  /* ret[0] = a0^2 + 2*(a1*a2)*(u+1) */
  //mul_by_u_plus_1_fp2(ret[0], m12);
 cy_mul_by_u_plus_1_fp2( &m12, &ret[0]);

//  add_fp2(ret[0], ret[0], s0);
 cy_fp2_add(&ret[0], &s0, &ret[0]);

  /* ret[1] = a2^2*(u+1) + 2*(a0*a1) */
//  mul_by_u_plus_1_fp2(ret[1], s2);
 cy_mul_by_u_plus_1_fp2(&ret[1], &s2);
  //add_fp2(ret[1], ret[1], m01);
 cy_fp2_add(&ret[1], &m01, &ret[1]);


 cy_fp2_free( &s0 );
 cy_fp2_free( &m01 );
 cy_fp2_free( &s2 );
 cy_fp2_free( &m12 );

 end:
 	 return error;
}



static cy_error_t cy_fp6_add(const cy_fp6_t a, const cy_fp6_t b, cy_fp6_t ret)
{
	cy_error_t error;

//  add_fp2(ret[0], a[0], b[0]);
	CY_CHECK(cy_fp2_add(&a[0], &b[0], &ret[0]));

//  add_fp2(ret[1], a[1], b[1]);
	CY_CHECK(cy_fp2_add(&a[1], &b[1], &ret[1]));

//  add_fp2(ret[2], a[2], b[2]);
	CY_CHECK(cy_fp2_add(&a[1], &b[1], &ret[1]));

  end:
  	  return error;
}
static cy_error_t cy_fp6_sub(const cy_fp6_t a, const cy_fp6_t b, cy_fp6_t ret)
{
 cy_error_t error;

//  sub_fp2(ret[0], a[0], b[0]);
 CY_CHECK(cy_fp2_sub(&a[0], &b[0], &ret[0]));

//  sub_fp2(ret[1], a[1], b[1]);
  cy_fp2_sub(&a[1], &b[1], &ret[1]);

//  sub_fp2(ret[2], a[2], b[2]);
  cy_fp2_sub(&a[2], &b[2], &ret[2]);

  end:
  	  return error;
}



cy_error_t cy_fp6_neg( const cy_fp6_t a, cy_fp6_t ret)
{
  cy_error_t error;
//	neg_fp2(ret[0], a[0]);
  CY_CHECK(cy_fp2_neg(&a[0], &ret[0]));

  //neg_fp2(ret[1], a[1]);

  CY_CHECK(cy_fp2_neg(&a[1], &ret[1]));

  //neg_fp2(ret[2], a[2]);
  CY_CHECK(cy_fp2_neg(&a[2], &ret[2]));

  end:
  	  return error;
}

cy_error_t cy_fp6_alloc(fp2_ctx_t *ctx_fp2, cy_fp6_t *a){

	cy_error_t error = CY_KO;
	size_t j;

	for(j=0; j<3 ; j++){
				CY_CHECK(cy_fp2_alloc(ctx_fp2, &((*a)[j])) );
			}

	end:
	  return error;
}


cy_error_t cy_fp2x6_alloc(fp2_ctx_t *ctx_fp2, cy_fp12_t *a){

	cy_error_t error = CY_KO;
	size_t i,j;

	for(i=0;i<2;i++){
	for(j=0; j<3 ; j++){
				CY_CHECK(cy_fp2_alloc(ctx_fp2, &((*a)[i][j])) );
			}
	}
	end:
	  return error;
}

static cy_error_t cy_fp4_alloc(fp2_ctx_t *ctx_fp2, cy_fp4_t *a){

	cy_error_t error = CY_KO;
	size_t j;

	for(j=0; j<2 ; j++){
				CY_CHECK(cy_fp2_alloc(ctx_fp2, &((*a)[j])) );
			}

	end:
	  return error;
}


cy_error_t cy_fp4_free(cy_fp4_t *a){

	cy_error_t error = CY_KO;
	size_t j;

	for(j=0; j<2 ; j++){
				CY_CHECK(cy_fp2_free( &((*a)[j])) );
			}

	end:
	  return error;
}

cy_error_t cy_fp6_free(cy_fp6_t *a){

	cy_error_t error = CY_KO;
	size_t j;

	for(j=0; j<3 ; j++){
				CY_CHECK(cy_fp2_free( &((*a)[j])) );
			}

	end:
	  return error;
}

cy_error_t cy_fp2x3x2_alloc(fp12_ctx_t *ps_ctx_fp12, cy_fp12_t *out)
{
	cy_error_t error = CY_KO;
	size_t i,j;


	for(i=0;i<2;i++){
		for(j=0; j<3 ; j++){
			(*out)[0][0].ctx_quad=&ps_ctx_fp12->ctx_quad;
			CY_CHECK(cy_fp2_alloc(&ps_ctx_fp12->ctx_quad, out[i][j]) );
		}
	}

	end:
	  return error;
}


/*
 * Fp12 extension
 */
/*karatsuba like, assuming complex representation of quad extension*/
cy_error_t cy_fp2x3x2_mul(const cy_fp12_t *a, const cy_fp12_t *b,  cy_fp12_t *ret)
{
  cy_error_t error = CY_KO;

  cy_fp6_t t0, t1, t2;
  fp2_ctx_t *ctx_fp2=((*a)[0][0]).ctx_quad;


  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t1 ));
  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t2 ));


  cy_fp_ctx_t *ctx = (*a)[0][0].ctx_fp;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  //mul_fp6(t0, a[0], b[0]);
  cy_fp6_mul((*a)[0], (*b)[0], t0);

   //mul_fp6(t1, a[1], b[1]);
  cy_fp6_mul((*a)[1], (*b)[1], t1);

   /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1
             = a0*b1 + a1*b0 */
//   add_fp6(t2, a[0], a[1]);
  cy_fp6_add((*a)[0], (*a)[1], t2);

  // add_fp6(ret[1], b[0], b[1]);
  cy_fp6_add( (*b)[0], (*b)[1], (*ret)[1]);

//   mul_fp6(ret[1], ret[1], t2);
  cy_fp6_mul((*ret)[1], t2, (*ret)[1]);

//   sub_fp6(ret[1], ret[1], t0);
  cy_fp6_mul((*ret)[1], t0, (*ret)[1]);

//   sub_fp6(ret[1], ret[1], t1);
  cy_fp6_mul((*ret)[1], t1, (*ret)[1]);

   /* ret[0] = a0*b0 + a1*b1*v */
   //mul_by_u_plus_1_fp2(t1[2], t1[2]);
  cy_mul_by_u_plus_1_fp2(&t1[2], &t1[2]);

 //  add_fp2(ret[0][0], t0[0], t1[2]);
  cy_fp2_add(&(t0[0]), &(t1[2]), &((*ret)[0][0]) );

//   add_fp2(ret[0][1], t0[1], t1[0]);
  cy_fp2_add(&(t0[1]), &(t1[0]), &((*ret)[0][1]) );

//   add_fp2(ret[0][2], t0[2], t1[1]);
  cy_fp2_add(&(t1[1]), &(t0[2]), &((*ret)[0][2]) );


   cy_fp6_free( &t0 );
   cy_fp6_free( &t1 );
   cy_fp6_free( &t2 );


end:
  return error;
}

cy_error_t cy_fp2x3x2_add(cy_fp12_t *a, const cy_fp12_t *b, cy_fp12_t *out)
{
  cy_error_t error = CY_KO;
  size_t i,j;

  fp2_ctx_t *ctx=((*a)[0][0]).ctx_quad;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }


	for(i=0;i<2;i++){
		for(j=0;j<3;j++){
			CY_CHECK(cy_fp2_sub(a[i][j], b[i][j], out[i][j] ));
		}
	}



end:
  return error;
}

cy_error_t cy_fp2x3x2_set_zero(cy_fp12_t *out)
{
	  cy_error_t error = CY_KO;
	  size_t i,j;
	  fp2_ctx_t *ctx=((*out)[0][0]).ctx_quad;

	    if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	      error = CY_ERR_UNIT;

	      goto end;
	    }

	    for(i=0;i<2;i++){
	    		for(j=0;j<3;j++){
	    			CY_CHECK(cy_fp2_set_zero(&(*out)[i][j] ));
	    		}
	    	}

	    end:
		 return error;
}

cy_error_t cy_fp2x3x2_sub(cy_fp12_t *a, const cy_fp12_t *b, cy_fp12_t *out)
{

	  cy_error_t error = CY_KO;


	  fp2_ctx_t *ctx=((*a)[0][0]).ctx_quad;

	  size_t i,j;
	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;

	    goto end;
	  }

		for(i=0;i<2;i++){
			for(j=0;j<3;j++){
				CY_CHECK(cy_fp2_sub(a[i][j], b[i][j], out[i][j] ));
			}
		}

	end:
	  return error;
}



static inline cy_error_t mul_by_0y0_fp6( const cy_fp6_t a,
                                  const cy_fp2_t b, cy_fp6_t ret)
{
cy_error_t error;
  fp2_ctx_t *ctx = (a)[0].ctx_quad;

 // vec384x t;
 cy_fp6_t t;


 CY_CHECK(cy_fp6_alloc(ctx, &t ));

 // mul_fp2(t, a[2], b);
 cy_fp2_mul(&(a[2]), &b, &(ret[1]));

//  mul_fp2(ret[2], a[1], b);
 cy_fp2_mul(&a[1], &b, &ret[2]);

//  mul_fp2(ret[1], a[0], b);
 cy_fp2_mul(&a[0], &b, &ret[1]);

//  mul_by_u_plus_1_fp2(ret[0], t);
 cy_mul_by_u_plus_1_fp2( &t[0], &ret[0]);
 end:
  return error;
}


static inline cy_error_t cy_mul_by_0y0_fp6(const cy_fp6_t a,
                                  const cy_fp2_t b, cy_fp6_t ret)
{
  cy_error_t error;

  cy_fp2_t t;
  fp2_ctx_t *ctx_fp2=((	a)[0]).ctx_quad;


   CY_CHECK(cy_fp2_alloc(ctx_fp2, &t ));

  //mul_fp2(t, a[2], b);
  cy_fp2_mul(&a[2], &b, &t);

  //mul_fp2(ret[2], a[1], b);
  cy_fp2_mul( &a[1], &b, &ret[2]);

  //mul_fp2(ret[1], a[0], b);
  cy_fp2_mul( &a[0], &b, &ret[1]);

  //mul_by_u_plus_1_fp2(ret[0], t);
  cy_mul_by_u_plus_1_fp2(&t, &(ret[0]));

  CY_CHECK(cy_fp2_free( &t ));

  end:
   return error;
}

static cy_error_t cy_mul_by_xy0_fp6( const cy_fp6_t a, const cy_fp6_t  b, cy_fp6_t  ret)
{
 cy_error_t error;

 fp2_ctx_t *ctx_fp2=((	a)[0]).ctx_quad;

//  vec384x t0, t1, /*t2,*/ t3, t4, t5;
  cy_fp2_t t0, t1, /*t2,*/ t3, t4, t5;
 //todo: allocations
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t1 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t3 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t4 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t5 ));

//  mul_fp2(t0, a[0], b[0]);
  cy_fp2_mul( &a[0], &b[0], &t0);

//  mul_fp2(t1, a[1], b[1]);
  cy_fp2_mul( &a[1], &b[1], &t1);

  /* ret[0] = ((a1 + a2)*(b1 + 0) - a1*b1 - a2*0)*(u+1) + a0*b0
            = (a1*0 + a2*b1)*(u+1) + a0*b0 */
  //mul_fp2(t3, a[2], b[1]);
  cy_fp2_mul( &a[2], &b[1], &t3);

//  mul_by_u_plus_1_fp2(t3, t3);
  cy_mul_by_u_plus_1_fp2(&t3, &t3);
  /* add_fp2(ret[0], t3, t0); considering possible aliasing... */

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1 + a2*0*(u+1)
            = a0*b1 + a1*b0 + a2*0*(u+1) */
//  add_fp2(t4, a[0], a[1]);
  cy_fp2_add( &a[0], &a[1], &t4);

//  add_fp2(t5, b[0], b[1]);
  cy_fp2_add( &b[0], &b[1], &t5);

//  mul_fp2(ret[1], t4, t5);
  cy_fp2_mul( &t5, &t4, &ret[1]);

//  sub_fp2(ret[1], ret[1], t0);
  cy_fp2_sub( &ret[1], &t0, &ret[1]);

//  sub_fp2(ret[1], ret[1], t1);
  cy_fp2_sub( &ret[1], &t1, &ret[1]);

  /* ret[2] = (a0 + a2)*(b0 + 0) - a0*b0 - a2*0 + a1*b1
            = a0*0 + a2*b0 + a1*b1 */
//  mul_fp2(ret[2], a[2], b[0]);
  cy_fp2_mul( &b[0], &a[2], &ret[2]);

//  add_fp2(ret[2], ret[2], t1);
  cy_fp2_add( &ret[2], &t1, &ret[2]);

//  add_fp2(ret[0], t3, t0); /* ... moved from above */
  cy_fp2_add( &t0, &t3, &ret[0]);

  CY_CHECK(cy_fp2_free( &t0 ));
  CY_CHECK(cy_fp2_free( &t1 ));
  CY_CHECK(cy_fp2_free( &t3 ));
  CY_CHECK(cy_fp2_free( &t4 ));
  CY_CHECK(cy_fp2_free( &t5 ));

  end:
   return error;
}

cy_error_t cy_mul_by_xy00z0_fp12(cy_fp12_t *ret, const cy_fp12_t *a,
                               const cy_fp6_t xy00z0)
{
	cy_error_t error;
  //vec384fp6 t0, t1, t2;
  cy_fp6_t t0, t1, t2;
  fp2_ctx_t *ctx_fp2=((*a)[0][0]).ctx_quad;


  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t1 ));
  CY_CHECK(cy_fp6_alloc(ctx_fp2, &t2 ));

  //mul_by_xy0_fp6(t0, a[0], xy00z0);
  cy_mul_by_xy0_fp6( (*a)[0], (xy00z0), t0);


  //mul_by_0y0_fp6(t1, a[1], xy00z0[2]);
  cy_mul_by_0y0_fp6( ((*a)[1]), (xy00z0)[2], t1);

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1
            = a0*b1 + a1*b0 */
  //vec_copy(t2[0], xy00z0[0], sizeof(t2[0]));
  cy_fp2_copy(&((xy00z0)[0]), &t2[0]);

 // add_fp2(t2[1], xy00z0[1], xy00z0[2]);
  cy_fp2_add(&xy00z0[1], &xy00z0[2], &t2[1]);

  //add_fp6(ret[1], a[0], a[1]);
  cy_fp6_add((*a)[0], (*a)[1], (*ret)[1]);

//  mul_by_xy0_fp6(ret[1], ret[1], t2);
  cy_mul_by_xy0_fp6((*ret)[1], t2, (*ret)[1]);

//  sub_fp6(ret[1], ret[1], t0);
  cy_fp6_sub( (*ret)[1], t0, (*ret)[1]);

//  sub_fp6(ret[1], ret[1], t1);
  cy_fp6_sub((*ret)[1], t1, (*ret)[1]);

  /* ret[0] = a0*b0 + a1*b1*v */
  //mul_by_u_plus_1_fp2(t1[2], t1[2]);
  cy_mul_by_u_plus_1_fp2(&t1[2], &t1[2]);

  //add_fp2(ret[0][0], t0[0], t1[2]);
  cy_fp2_add(&t0[0], &t1[2], &((*ret)[0][0]));

//  add_fp2(ret[0][1], t0[1], t1[0]);
  cy_fp2_add(&t1[0], &t0[1], &((*ret)[0][1]));

//  add_fp2(ret[0][2], t0[2], t1[1]);
  cy_fp2_add(&t1[1],&t0[2], &((*ret)[0][2]));

  CY_CHECK(cy_fp6_free( &t0 ));
  CY_CHECK(cy_fp6_free( &t1 ));
  CY_CHECK(cy_fp6_free( &t2 ));

  end:
   return error;
}


cy_error_t cy_fp2x3x2_sqr( const cy_fp12_t *a, cy_fp12_t *ret)
{
	cy_error_t error;
//  vec384fp6 t0, t1;
  cy_fp6_t t0, t1;

  fp2_ctx_t *ctx_fp2=((*a)[0][0]).ctx_quad;
   CY_CHECK(cy_fp6_alloc(ctx_fp2, &t0 ));
   CY_CHECK(cy_fp6_alloc(ctx_fp2, &t1 ));

//  add_fp6(t0, a[0], a[1]);
  cy_fp6_add((*a)[0], (*a)[1], t0);

//  mul_by_u_plus_1_fp2(t1[2], a[1][2]);
  cy_mul_by_u_plus_1_fp2( &((*a)[1][2]), &t1[2]);

//  add_fp2(t1[0], a[0][0], t1[2]);
  cy_fp2_add(&((*a)[0][0]), &t1[2], &t1[0]);

//  add_fp2(t1[1], a[0][1], a[1][0]);
  cy_fp2_add(&((*a)[0][1]), &((*a)[1][0]), &t1[1]);

//  add_fp2(t1[2], a[0][2], a[1][1]);
  cy_fp2_add( &((*a)[0][2]), &((*a)[1][1]), &t1[2]);

//  mul_fp6(t0, t0, t1);
  cy_fp6_mul(t0, t1, t0);

 // mul_fp6(t1, a[0], a[1]);
  cy_fp6_mul((*a)[0], (*a)[1], t1);

  /* ret[1] = 2*(a0*a1) */
//  add_fp6(ret[1], t1, t1);
  cy_fp6_add(t1, t1, ((*ret)[1]));

  /* ret[0] = (a0 + a1)*(a0 + a1*v) - a0*a1 - a0*a1*v
            = a0^2 + a1^2*v */
 // sub_fp6(ret[0], t0, t1);
  cy_fp6_sub(t0, t1, ((*ret)[0]));

//  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  cy_mul_by_u_plus_1_fp2(&t1[2], &t1[2]);

//  sub_fp2(ret[0][0], ret[0][0], t1[2]);
  cy_fp2_sub(&((*ret)[0][0]), &t1[2], &((*ret)[0][0]));

//  sub_fp2(ret[0][1], ret[0][1], t1[0]);
  cy_fp2_sub(&((*ret)[0][1]), &t1[0], &((*ret)[0][1]));

//  sub_fp2(ret[0][2], ret[0][2], t1[1]);
  cy_fp2_sub(&((*ret)[0][2]), &t1[0], &((*ret)[0][2]));

  CY_CHECK(cy_fp6_free( &t0 ));
  CY_CHECK(cy_fp6_free( &t1 ));

  end:
   return error;
}


#define conjugate_fp12(a)  cy_fp6_neg(a[1], a[1])


static cy_error_t cy_fp6_inv(const cy_fp6_t a, cy_fp6_t ret)
{
	cy_error_t error;
//  vec384x c0, c1, c2, t0, t1;
  cy_fp2_t c0, c1, c2, t0, t1;
  fp2_ctx_t *ctx_fp2=((a)[0]).ctx_quad;

  CY_CHECK(cy_fp2_alloc(ctx_fp2, &c0 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &c1 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &c2 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t1 ));

  /* c0 = a0^2 - (a1*a2)*(u+1) */
  //sqr_fp2(c0, a[0]);
  cy_fp2_sqr(&a[0], &c0);

//  mul_fp2(t0, a[1], a[2]);
  cy_fp2_mul(&a[1], &a[2], &c0);

//  mul_by_u_plus_1_fp2(t0, t0);
  cy_mul_by_u_plus_1_fp2(&t0, &t0);

//  sub_fp2(c0, c0, t0);
  cy_fp2_sub(&c0, &t0, &c0);

  /* c1 = a2^2*(u+1) - (a0*a1) */
//  sqr_fp2(c1, a[2]);
  cy_fp2_sqr(&a[2], &c1);

//  mul_by_u_plus_1_fp2(c1, c1);
  cy_mul_by_u_plus_1_fp2(&c1, &c1);

//  mul_fp2(t0, a[0], a[1]);
  cy_fp2_mul(&a[0], &a[1], &t0);

  //sub_fp2(c1, c1, t0);
  cy_fp2_sub(&c1,  &t0, &c1);

  /* c2 = a1^2 - a0*a2 */
//  sqr_fp2(c2, a[1]);
  cy_fp2_sqr( &a[1], &c2);

//  mul_fp2(t0, a[0], a[2]);
  cy_fp2_mul(&a[0], &a[2], &t0);

//  sub_fp2(c2, c2, t0);
  cy_fp2_sub(&c1,  &t0, &c2);

  /* (a2*c1 + a1*c2)*(u+1) + a0*c0 */
//  mul_fp2(t0, c1, a[2]);
  cy_fp2_mul(&c1, &a[2], &t0);

  //mul_fp2(t1, c2, a[1]);
  cy_fp2_mul(&c2, &a[1], &t1);

//  add_fp2(t0, t0, t1);
  cy_fp2_add(&t0,  &t1, &t0);

//  mul_by_u_plus_1_fp2(t0, t0);
  cy_mul_by_u_plus_1_fp2(&t0, &t0);

  //mul_fp2(t1, c0, a[0]);
  cy_fp2_mul(&c0, &a[0], &t1);

  //add_fp2(t0, t0, t1);
  cy_fp2_add(&t0,  &t1, &t0);

  //reciprocal_fp2(t1, t0);
  cy_fp2_inv(&t0, &t1);

//  mul_fp2(ret[0], c0, t1);
  cy_fp2_mul(&c0,  &t1, &ret[0]);

//  mul_fp2(ret[1], c1, t1);
  cy_fp2_mul(&c1,  &t1, &ret[1]);

  //mul_fp2(ret[2], c2, t1);
  cy_fp2_mul(&c2,  &t1, &ret[2]);

  CY_CHECK(cy_fp2_free( &c0 ));
  CY_CHECK(cy_fp2_free( &c1 ));
  CY_CHECK(cy_fp2_free( &c2 ));
  CY_CHECK(cy_fp2_free( &t0 ));
  CY_CHECK(cy_fp2_free( &t1 ));

  end:
   return error;
}


cy_error_t cy_fp2x3x2_inv(const cy_fp12_t *a, cy_fp12_t *ret)
{
	cy_error_t error;
	cy_fp6_t t0, t1;

	fp2_ctx_t *ctx_fp2=((*a)[0][0]).ctx_quad;
	CY_CHECK(cy_fp6_alloc(ctx_fp2, &t0 ));
	CY_CHECK(cy_fp6_alloc(ctx_fp2, &t1 ));

//  sqr_fp6(t0, a[0]);
  cy_fp6_sqr((*a)[0],  t0);
//  sqr_fp6(t1, a[1]);
  cy_fp6_sqr((*a)[1],  t1);

//  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  cy_mul_by_u_plus_1_fp2(&t1[2], &t1[2]);

 // sub_fp2(t0[0], t0[0], t1[2]);
  cy_fp2_sub(&t0[0], &t1[2], &t0[0]);

 // sub_fp2(t0[1], t0[1], t1[0]);
  cy_fp2_sub(&t0[1], &t1[0], &t0[1]);

//  sub_fp2(t0[2], t0[2], t1[1]);
  cy_fp2_sub(&t0[2], &t1[1], &t0[2]);

//  inverse_fp6(t1, t0);
  cy_fp6_inv( t0, t1);

 // mul_fp6(ret[0], a[0], t1);
  cy_fp6_mul(((*a)[0]),  t1, ((*ret)[0]));

//  mul_fp6(ret[1], a[1], t1);
  cy_fp6_mul(((*a)[1]),  t1, ((*ret)[1]));

//  neg_fp6(ret[1], ret[1]);
  cy_fp6_neg(((*ret)[1]), ((*ret)[1]));

  CY_CHECK(cy_fp6_free( &t0 ));
  CY_CHECK(cy_fp6_free( &t1 ));

  end:
   return error;
}


static cy_error_t cy_fp4_sqr( const cy_fp2_t *a0, const cy_fp2_t *a1, cy_fp4_t ret)
{
  cy_error_t error;
  cy_fp2_t t0, t1;
  fp2_ctx_t *ctx_fp2=a0->ctx_quad;

  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp2_alloc(ctx_fp2, &t1 ));


//  sqr_fp2(t0, a0);
  cy_fp2_sqr(a0, &t0);

//  sqr_fp2(t1, a1);
  cy_fp2_sqr(a1, &t1);

  //add_fp2(ret[1], a0, a1);
  cy_fp2_mul(a0,  a1, &ret[1]);

//  mul_by_u_plus_1_fp2(ret[0], t1);
  cy_mul_by_u_plus_1_fp2( &t1, &ret[0]);

//  add_fp2(ret[0], ret[0], t0);
  cy_fp2_add(&ret[0],  &t0, &ret[0]);

//  sqr_fp2(ret[1], ret[1]);
  cy_fp2_sqr(&ret[1], &ret[1]);

//  sub_fp2(ret[1], ret[1], t0);
  cy_fp2_sub(&ret[1], &t0, &ret[1]);

  //sub_fp2(ret[1], ret[1], t1);
  cy_fp2_sub(&ret[1], &t1, &ret[1]);

  CY_CHECK(cy_fp2_free( &t0 ));
  CY_CHECK(cy_fp2_free( &t1 ));

  end:
   return error;
}


cy_error_t cy_fp2x3x2_cyclotomic_sqr(const cy_fp12_t *a, cy_fp12_t *ret)
{
	cy_error_t error;

  cy_fp4_t t0, t1, t2;

  fp2_ctx_t *ctx_fp2=( (*a)[0][0]).ctx_quad;


  CY_CHECK(cy_fp4_alloc(ctx_fp2, &t0 ));
  CY_CHECK(cy_fp4_alloc(ctx_fp2, &t1 ));

  //sqr_fp4(t0, a[0][0], a[1][1]);
  CY_CHECK(cy_fp4_sqr( &((*a)[0][0]), &((*a)[1][1]), t0));

//  sqr_fp4(t1, a[1][0], a[0][2]);
  cy_fp4_sqr( &((*a)[1][0]), &((*a)[0][2]), t1);

//  sqr_fp4(t2, a[0][1], a[1][2]);
  cy_fp4_sqr( &((*a)[0][1]), &((*a)[1][2]), t2);

//  sub_fp2(ret[0][0], t0[0], a[0][0]);
  cy_fp2_sub(  &t0[0], &((*a)[0][0]), &((*ret)[0][0]));

//  add_fp2(ret[0][0], ret[0][0], ret[0][0]);
  cy_fp2_sub( &((*ret)[0][0]), &((*ret)[0][0]), &((*ret)[0][0]));

 // add_fp2(ret[0][0], ret[0][0], t0[0]);
  cy_fp2_add( &((*ret)[0][0]), & t0[0], &((*ret)[0][0]));

  //sub_fp2(ret[0][1], t1[0], a[0][1]);
  cy_fp2_sub( &((*a)[0][1]), & t1[0],  &((*ret)[0][1]));

//  add_fp2(ret[0][1], ret[0][1], ret[0][1]);
  cy_fp2_add(  &((*ret)[0][1]),  &((*ret)[0][1]),  &((*ret)[0][1]));

//  add_fp2(ret[0][1], ret[0][1], t1[0]);
  cy_fp2_add( &((*ret)[0][1]), & t1[0], &((*ret)[0][1]));

//  sub_fp2(ret[0][2], t2[0], a[0][2]);
  cy_fp2_sub( &((*a)[0][2]), & t2[0],  &((*ret)[0][2]));

//  add_fp2(ret[0][2], ret[0][2], ret[0][2]);
  cy_fp2_add(  &((*ret)[0][2]),  &((*ret)[0][2]),  &((*ret)[0][2]));

//  add_fp2(ret[0][2], ret[0][2], t2[0]);
  cy_fp2_add(  &((*ret)[0][2]),  &t2[0],  &((*ret)[0][2]));

//  mul_by_u_plus_1_fp2(t2[1], t2[1]);
  cy_mul_by_u_plus_1_fp2(&t2[1], &t2[1]);

//  add_fp2(ret[1][0], t2[1], a[1][0]);
  cy_fp2_add( &t2[1], a[1][0], &((*ret)[1][0]));

//  add_fp2(ret[1][0], ret[1][0], ret[1][0]);
  cy_fp2_add( &((*ret)[1][0]), &((*ret)[1][0]), &((*ret)[1][0]));

//  add_fp2(ret[1][0], ret[1][0], t2[1]);
  cy_fp2_add(  &((*ret)[1][0]), &t2[1],  &((*ret)[1][0]));

//  add_fp2(ret[1][1], t0[1], a[1][1]);
  cy_fp2_add( &t0[1], &((*a)[1][1]),  &((*ret)[1][1]));

//  add_fp2(ret[1][1], ret[1][1], ret[1][1]);
  cy_fp2_add( &((*ret)[1][1]), &((*ret)[1][1]),  &((*ret)[1][1]));

 // add_fp2(ret[1][1], ret[1][1], t0[1]);
  cy_fp2_add( &((*ret)[1][1]), &t0[1], &((*ret)[1][1]));

//  add_fp2(ret[1][2], t1[1], a[1][2]);
  cy_fp2_add( &((*a)[1][2]), &t1[1], &((*ret)[1][2]));

 // add_fp2(ret[1][2], ret[1][2], ret[1][2]);
  cy_fp2_add( &((*ret)[1][2]), &((*ret)[1][2]),  &((*ret)[1][2]));

//  add_fp2(ret[1][2], ret[1][2], t1[1]);
  cy_fp2_add( &((*ret)[1][2]), &t1[1],  &((*ret)[1][2]));

  end:
   return error;

}
