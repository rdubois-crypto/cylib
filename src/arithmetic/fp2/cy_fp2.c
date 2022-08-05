/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_fp2.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: cy_lib native quadratic field extensions implementation */
/**********************************************************************************/

#include "cy_fp2.h"
#include "cy_fp.h"

cy_error_t cy_fp2_init(fp2_ctx_t *ps_ctx_fp2, uint8_t *pu8_Mem,
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


  if(ps_ctx_fp2->ctx->is_initialized!=CY_LIB_INITIALIZED)
  {
	  CY_CHECK(cy_fp_init(ps_ctx_fp2->ctx, pu8_Mem, t8_Memory, argc, argv));
  }

  ps_ctx_fp2->offset = ps_ctx_fp2->ctx->offset;

end:
  return error;
}

/*karatsuba like, assuming complex representation of quad extension*/
cy_error_t cy_fp2_mul(cy_quad_t *ret, const cy_fp2_t *a, const cy_fp2_t *b)
{
  cy_error_t error = CY_KO;
  cy_fp2_t aa, bb, cc;
  cy_error_t error = CY_KO;
  fp_ctx_t *ctx = a->ctx;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;

    goto end;
  }

  CY_CHECK(cy_fp_add(a[0], a[1], aa));
  CY_CHECK(cy_fp_add(b[0], b[1], bb));
  CY_CHECK(cy_fp_mult_mont(bb, aa, bb));
  CY_CHECK(cy_fp_mult_mont(a[0], b[0], aa));
  CY_CHECK(cy_fp_mult_mont(a[1], b[1], cc));
  CY_CHECK(cy_fp_sub(aa, cc, ret[0]));
  CY_CHECK(cy_fp_sub(bb, aa, ret[1]));
  CY_CHECK(cy_fp_sub(ret[1], cc, ret[1]));

end:
  return error;
}

cy_error_t cy_fp2_add(cy_fp2_t *ret, const cy_fp2_t *a, const cy_fp2_t *b)
{
  cy_error_t error = CY_KO;
  cy_fp2_t aa, bb, cc;
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
