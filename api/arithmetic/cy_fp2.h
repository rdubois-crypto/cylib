/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_fp2.h										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Quadratic extensions Integers API                                 */
/**********************************************************************************/

#ifndef _CY_FP2_H
#define _CY_FP2_H


struct fp2_ctx_s{
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;

  size_t t8_modular_p;
  fp_ctx_t *ctx;

  void *modular_p; /*pointer to modular context in Shared Memory*/
  cy_fp_t alpha; /* the value of X^2+alpha, the reducing polynomial*/
};

typedef struct fp2_ctx_s fp2_ctx_t;

struct cy_fp2_s{
 cy_fp_t *x;
 cy_fp_t *y;
 fp2_ctx_t *ctx;

};



typedef struct cy_fp2_s cy_fp2_t;


extern cy_error_t cy_fp2_init(fp2_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]);

extern cy_error_t cy_fp2_alloc(fp2_ctx_t *ps_ctx,  size_t fp2_t8, cy_fp2_t *out );
extern cy_error_t cy_fp2_import(fp2_ctx_t *ps_ctx,  uint8_t *in, size_t fp2_t8, cy_fp2_t *out );
extern cy_error_t cy_fp2_add(fp2_ctx_t *ctx,  cy_fp2_t *a, cy_fp2_t *b, cy_fp2_t *r);

extern cy_error_t cy_fp2_sub(fp2_ctx_t *ctx,  cy_fp2_t *a, cy_fp2_t *b, cy_fp2_t *r);

extern cy_error_t cy_fp2_mul(fp2_ctx_t *ctx,  cy_fp2_t *a, cy_fp2_t *b, cy_fp2_t *r);


extern cy_error_t cy_fp2_free(cy_fp2_t *fp2);

extern cy_error_t cy_fp2_uninit(fp2_ctx_t *ctx, uint8_t *pu8_Mem , const size_t t8_Memory);



#endif
