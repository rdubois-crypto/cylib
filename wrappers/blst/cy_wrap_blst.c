/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_blst.h*/
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Library initialization and management API */
/**********************************************************************************/

#include "cy_wrap_blst.h"
#include "blst_fields.h"
#include "cy_errors.h"

cy_error_t wrap_blst_fp_add(cy_fp_ctx_t *ctx, *a, *b, *r)
{

  UNUSED(ctx);
  add_fp(r, a, b);
  return CY_OK;
}

cy_error_t wrap_blst_fp_sub(cy_fp_ctx_t *ctx, *a, *b, *r)
{

  UNUSED(ctx);
  sub_fp(r, a, b);
  return CY_OK;
}

cy_error_t wrap_blst_fp_mult_mont(cy_fp_ctx_t *ctx, *a, *b, *r)
{

  UNUSED(ctx);
  mul_fp(r, a, b);

  return CY_OK;
}
