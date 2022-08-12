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

#include "cy_configuration.h"
#include "cy_quad.h"
#include "cy_fp2.h"


/* multiplication by u+1, assuming -1 is square mod p */
static inline cy_error_t mul_by_u_plus_1_fp2(const cy_fp2_t *a, cy_fp2_t ret)
{
  cy_error_t error = CY_KO;

  CY_CHECK(cy_fp_add(a->x, a->y, ret->y));
  CY_CHECK(cy_fp_sub(a->x, a->y, ret->x));


  end:
    return error;
}


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


  if(ps_ctx_fp12->ctx->is_initialized!=CY_LIB_INITIALIZED)
  {
	  CY_CHECK(cy_fp2_init(ps_ctx_fp12->ctx_fp2, pu8_Mem, t8_Memory, argc, argv));
  }

end:
  return error;
}

cy_error_t cy_fp2x3x2_alloc(fp12_ctx_t *ps_ctx_fp12, cy_fp12_t *out)
{
	cy_error_t error = CY_KO;
	size_t i;

	for(i=0;i<2;i++){
		for(j=0; j<3 ; j++){
			CY_CHECK(cy_fp2_alloc(ps_ctx_fp12->ctx_fp2, ps_ctx_fp12->ctx_fp->t8_modular, out[i][j]) );
		}
	}

	end:
	  return error;
}


/*karatsuba like, assuming complex representation of quad extension*/
cy_error_t cy_fp2x3x2_mul(cy_fp12_t *a, const cy_fp12_t *b, const cy_fp12_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp12_t aa, bb, cc;
  cy_error_t error = CY_KO;
  fp_ctx_t *ctx = a->ctx;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }



end:
  return error;
}

cy_error_t cy_fp2x3x2_add(cy_fp12_t *a, const cy_fp12_t *b, const cy_fp12_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp12_t aa, bb, cc;
  cy_error_t error = CY_KO;
  fp2_ctx_t *ctx = a[0][0]->ctx_fp2;
  size_t i,j;
  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

	for(i=0;i<2;i++){
		for(j=0;j<3;j++){
			CY_CHECK(cy_fp2_add(a[i][j], b[i][j], out[i][j] ));
		}
	}

end:
  return error;
}


cy_error_t cy_fp2x3x2_sub(cy_fp12_t *a, const cy_fp12_t *b, const cy_fp12_t *out)
{

	  cy_error_t error = CY_KO;
	  cy_fp12_t aa, bb, cc;
	  cy_error_t error = CY_KO;
	  fp2_ctx_t *ctx = a[0][0]->ctx_fp2;
	  size_t i,j;
	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;

	    goto end;
	  }

		for(i=0;i<2;i++){
			for(j=0;j<3;j++){
				CY_CHECK(cy_fp2_add(a[i][j], b[i][j], out[i][j] ));
			}
		}

	end:
	  return error;
}


