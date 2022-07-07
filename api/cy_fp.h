/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_word64.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Modular Integers API                                              */
/**********************************************************************************/


#ifndef API_CY_FP_H_
#define API_CY_FP_H_

#include "cy_errors.h"
/*include at last a native or wrap API*/


struct fp_ctx_s{
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;

  size_t t8_modular;
  void *modular;
};

typedef struct fp_ctx_s fp_ctx_t;

extern cy_error_t cy_fp_init(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]);

cy_error_t cy_fp_add(fp_ctx_t *ctx,  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
cy_error_t cy_fp_sub(fp_ctx_t *ctx,  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
cy_error_t cy_fp_mult_mont(fp_ctx_t *ctx,  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);

cy_error_t cy_fp_destroy(fp_ctx_t *ctx);


#endif /* API_CY_FP_H_ */
