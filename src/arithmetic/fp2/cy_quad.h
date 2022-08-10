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


struct quad_ctx_s {
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  size_t offset;

  size_t t8_modular_p;
  fp_ctx_t *ctx;

  void *modular_p; /*pointer to modular context in Shared Memory*/
};

typedef struct quad_ctx_s quad_ctx_t;

struct cy_quad_s {
  cy_fp_t *x;
  cy_fp_t *y;
  fp_ctx_t *ctx_fp;
  quad_ctx_t *ctx_quad;

};

typedef struct cy_quad_s cy_quad_t;

#define cy_fp2_t cy_quad_t
#define fp2_ctx_t quad_ctx_t

/* wrappers */

#define cy_fp2_init(a, b, c, d, e) (cy_quad_init(a, b, c, d, e))
#define cy_fp2_uninit(ctx, mem, size) (cy_quad_uninit(ctx, mem, size))



#endif /* SRC_INNOVATION_CY_QUAD_H_ */
