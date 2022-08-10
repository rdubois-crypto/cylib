/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_quad.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: cy_lib NATIVE quadratic field extensions implementation */
/**********************************************************************************/
#include <stddef.h>
#include <stdint.h>

#include "cy_configuration.h"
#include "cy_quad.h"
#include "cy_fp2.h"

cy_error_t cy_quad_init(quad_ctx_t *ps_ctx_quad, uint8_t *pu8_Mem,
                       const size_t t8_Memory, const int argc,
                       const uint8_t *argv[])
{
  cy_error_t error = CY_KO;

  /* the choice of includes indicates a library not supporting this quad implementation (-1 has to be a square root) */
  #ifdef _FP2_UNIT_DISABLED
  return CY_NOT_IMPLEMENTED;
  #endif

  if (argc != 2) {
    return CY_KO;
  }
  /*control memory limit*/
  if (t8_Memory > _MAX_MEMORY) {
    error = CY_KO;
    goto end;
  }


  if(ps_ctx_quad->ctx->is_initialized!=CY_LIB_INITIALIZED)
  {
	  CY_CHECK(cy_fp_init(ps_ctx_quad->ctx, pu8_Mem, t8_Memory, argc, argv));
  }

  /* not sure that it is useful*/
  ps_ctx_quad->offset = ps_ctx_quad->ctx->offset;


end:
  return error;
}



/*karatsuba like, assuming complex representation of quad extension, from mul_mont_384x of blst*/
cy_error_t cy_quad_mul(const cy_quad_t *a, const cy_quad_t *b, cy_quad_t *ret)
{
  cy_error_t error = CY_KO;
  cy_fp_t aa, bb, cc;
  fp_ctx_t *ctx = a->ctx_fp;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }
  /* todo: allocations and free */

  CY_CHECK(cy_fp_add(a->x, a->y, &aa));
  CY_CHECK(cy_fp_add(b->x, b->y, &bb));
  CY_CHECK(cy_fp_mult_mont(&bb, &aa, &bb));
  CY_CHECK(cy_fp_mult_mont(a->x, b->x, &aa));
  CY_CHECK(cy_fp_mult_mont(a->y, b->y, &cc));
  CY_CHECK(cy_fp_sub(&aa, &cc, ret->x));
  CY_CHECK(cy_fp_sub(&bb, &aa, ret->y));
  CY_CHECK(cy_fp_sub(ret->x, &cc, ret->x));

end:
  return error;
}


/*karatsuba like, assuming complex representation of quad extension*/
cy_error_t cy_quad_sqr( const cy_quad_t *a, cy_quad_t *out_sqr)
{
  cy_error_t error = CY_KO;

  fp_ctx_t *ctx = a->ctx_fp;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_quad_mul(a, a, out_sqr));


end:
  return error;
}


cy_error_t cy_quad_add( const cy_quad_t *a, const cy_quad_t *b, cy_quad_t *ret)
{
  cy_error_t error = CY_KO;


  fp_ctx_t *ctx = a->ctx_fp;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_fp_add(a->x, b->x, ret->x));
  CY_CHECK(cy_fp_add(a->y, b->y, ret->y));

end:
  return error;
}


cy_error_t cy_quad_sub(cy_quad_t *ret, const cy_quad_t *a, const cy_quad_t *b)
{
  cy_error_t error = CY_KO;

  fp_ctx_t *ctx = a->ctx_fp;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_fp_sub(a->x, b->x, ret->x));
  CY_CHECK(cy_fp_sub(a->y, b->y, ret->y));

end:
  return error;
}


cy_error_t cy_quad_uninit(quad_ctx_t *ps_ctx_quad, uint8_t *pu8_Mem,
                       const size_t t8_Memory)
{
  cy_error_t error = CY_KO;


  if(ps_ctx_quad->ctx->is_initialized!=CY_LIB_INITIALIZED)
  {
	  error = CY_ERR_UNIT;
	   goto end;
  }

  CY_CHECK(cy_fp_uninit(ps_ctx_quad->ctx, pu8_Mem, t8_Memory));
  /* not sure that it is useful*/
  ps_ctx_quad->offset = ps_ctx_quad->ctx->offset;

  UNUSED(pu8_Mem);
  UNUSED(t8_Memory);

end:
  return error;
}

