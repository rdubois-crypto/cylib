/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_fp.h										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Modular Integers API                                              */
/**********************************************************************************/


#ifndef API_CY_FP_H_
#define API_CY_FP_H_

#include "cy_def.h"
#include "cy_errors.h"
#include "cy_memory_unit.h"
/*include at last a native or wrap API*/

#define _MEM_FP_RESERVED 0x55
#define _MEM_FP_ALLOCATED 0x99


struct fp_ctx_s{
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;

  size_t t8_modular;
  void *modular; /*pointer to modular context in Shared Memory*/
  void *montgomery_ctx;/*pointer to montgomery context in Shared Memory*/

};

typedef struct fp_ctx_s fp_ctx_t;


struct cy_fp_s{
  cy_flag_t is_initialized;

  uint8_t *index;
  fp_ctx_t *ctx;
  cy_bn_t *bn;

};


typedef struct cy_fp_s cy_fp_t;


/* fp unit handling */
extern cy_error_t cy_fp_init(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]);

extern cy_error_t cy_fp_uninit(fp_ctx_t *ctx, uint8_t *pu8_Mem , const size_t t8_Memory);


/* fp unit allocations */
extern cy_error_t cy_fp_alloc(fp_ctx_t *ps_ctx,  size_t fp_t8, cy_fp_t *out );
extern cy_error_t cy_fp_free(cy_fp_t *fp);


/* IO handling */
extern cy_error_t cy_fp_import( uint8_t *in, size_t fp_t8, cy_fp_t *out );
extern cy_error_t cy_fp_export(cy_fp_t *in, uint8_t *out , size_t t8_out);
extern cy_error_t cy_to_fp( uint8_t *in, size_t fp_t8,  DATA_FORMAT how, cy_fp_t *out );
extern cy_error_t cy_from_fp( uint8_t *in, size_t fp_t8,  DATA_FORMAT how, cy_fp_t *out );
extern cy_error_t cy_fp_set_zero(cy_fp_t *out);
extern cy_error_t cy_fp_set_one(cy_fp_t *out);

/* Arithmetic functions */
extern cy_error_t cy_fp_add(  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
extern cy_error_t cy_fp_sub( cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
extern cy_error_t cy_fp_mul(  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);

extern cy_error_t cy_fp_inv( cy_fp_t *in, cy_fp_t *out);
extern cy_error_t cy_fp_cmp(cy_fp_t *in1, cy_fp_t *in2, int *cmp);

/* Comparizons */
extern cy_error_t fp_iszero(const cy_fp_t *in, int *iszero);
extern cy_error_t fp_eq_or_opp(cy_fp_t *in1, cy_fp_t *in2, int *eq_or_opp);

/* Montgomery representation handling */
extern cy_error_t cy_fp_to_mont(fp_ctx_t *ctx,  cy_fp_t *in, cy_fp_t *out);
extern cy_error_t cy_fp_mult_mont( cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);


#endif /* API_CY_FP_H_ */
