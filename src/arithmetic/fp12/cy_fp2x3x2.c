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
/**********************************************************************************/

#include "cy_fp.h"
#include "cy_fp2.h"
#include "cy_fp12.h"


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
	  CY_CHECK(cy_fp_init(ps_ctx_fp12->ctx, pu8_Mem, t8_Memory, argc, argv));
  }

  /* not sure that it is useful*/
  ps_ctx_fp12->offset = ps_ctx_fp12->ctx->offset;

end:
  return error;
}



/*karatsuba like, assuming complex representation of quad extension*/
cy_error_t cy_fp2x3x2_mul(cy_quad_t *ret, const cy_fp12_t *a, const cy_fp12_t *b)
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

cy_error_t cy_fp2x3x2_add(cy_fp12_t *ret, const cy_fp12_t *a, const cy_fp12_t *b)
{
  cy_error_t error = CY_KO;
  cy_fp12_t aa, bb, cc;
  cy_error_t error = CY_KO;
  fp_ctx_t *ctx = a->ctx;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_fp_add(a->x, b->x, ret->x));
  CY_CHECK(cy_fp_add(a->y, b->y, ret->y));

end:
  return error;
}


cy_error_t cy_fp2x3x2_sub(cy_fp12_t *ret, const cy_fp12_t *a, const cy_fp12_t *b)
{
  cy_error_t error = CY_KO;
  cy_fp12_t aa, bb, cc;
  cy_error_t error = CY_KO;
  fp_ctx_t *ctx = a->ctx;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_fp_sub(a->x, b->x, ret->x));
  CY_CHECK(cy_fp_sub(a->y, b->y, ret->y));

end:
  return error;
}


