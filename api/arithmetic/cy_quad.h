/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_quad.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION:cy_lib NATIVE quadratic field extensions implementation header file*/
/**********************************************************************************/


#ifndef CY_QUAD_H_
#define CY_QUAD_H_


#define CY_FP2_LIBNAME "<CY_LIB FP2 Module>"

#define _HANDLED_FP2_MAX 8
#define _FP2_ZONE_T8 (_HANDLED_FP2_MAX*(sizeof(cy_fp_t)*sizeof(cy_bn_t)))

struct quad_ctx_s {
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  size_t offset;

  size_t t8_modular_p;
  cy_fp_ctx_t ctx;

  void *modular_p; /*pointer to modular context in Shared Memory*/
};

typedef struct quad_ctx_s quad_ctx_t;

struct cy_quad_s {
  cy_fp_t *x;
  cy_fp_t *y;
  cy_fp_ctx_t *ctx_fp;
  quad_ctx_t *ctx_quad;
  size_t index;
};

typedef struct cy_quad_s cy_quad_t;

#define cy_fp2_t cy_quad_t
#define fp2_ctx_t quad_ctx_t
#define ctx_fp2 ctx_quad

/* wrappers */

#define cy_fp2_init(a, b, c, d, e)       (cy_quad_init(a, b, c, d, e))

#define cy_fp2_inv(a, b)       (cy_quad_inv(a, b))
#define cy_fp2_copy(a, b)       (cy_quad_copy(a, b))


#define cy_fp2_uninit(ctx, mem, size)    (cy_quad_uninit(ctx, mem, size))
#define cy_fp2_alloc(ctx,   out)  (cy_quad_alloc(ctx,   out))

#define cy_fp2_import(in, s, out)        (cy_quad_import(in, s, out))
#define cy_fp2_set_zero(a) (cy_quad_set_zero(a))

#define cy_fp2_add(in1, in2, out) (cy_quad_add(in1, in2, out))
#define cy_fp2_sub(in1, in2, out) (cy_quad_sub(in1, in2, out))

#define cy_fp2_mul(in1, in2, out) (cy_quad_mul(in1, in2, out))
#define cy_fp2_sqr(in1,  out) (cy_quad_sqr(in1, out))

#define cy_fp2_neg(in, out) (cy_quad_neg(in, out))
#define cy_fp2_export(in, out, size) (cy_quad_export(in, out, size))

#define cy_fp2_free( out)  (cy_quad_free( out))
#endif /* SRC_INNOVATION_CY_QUAD_H_ */
