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

/* the ram max number of fp elements stacked*/
#define _HANDLED_FP_MAX 8
#define _FP_ZONE_T8 (_HANDLED_FP_MAX*sizeof(cy_bn_t))
#define _MEM_FP_RESERVED 0x55
#define _MEM_FP_ALLOCATED 0x99

#define _MAX_FP_T8 512 /* maximal byte size of a fp*/


struct cy_fp_ctx_s{
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;

  size_t t8_modular;
  void *modular; /*pointer to modular context in Shared Memory*/
  void *montgomery_ctx;/*pointer to montgomery context in Shared Memory*/

};

typedef struct cy_fp_ctx_s cy_fp_ctx_t;


struct cy_fp_s{
  cy_bn_t *bn;

  #ifndef _NO_CONTROL
  cy_flag_t is_initialized;
  #endif

  size_t index; /* offset to the object with the allocated Memory*/
  cy_fp_ctx_t *ctx;
};


typedef struct cy_fp_s cy_fp_t;


/* fp unit handling */
_CY_API extern  cy_error_t cy_fp_init(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_SecMem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]);

_CY_API extern cy_error_t cy_fp_uninit(cy_fp_ctx_t *ctx, uint8_t *pu8_SecMem , const size_t t8_Memory);


/* fp unit allocations */
_CY_API extern cy_error_t cy_fp_alloc(cy_fp_ctx_t *ps_ctx,  size_t fp_t8, cy_fp_t *out );
_CY_API extern cy_error_t cy_fp_free(cy_fp_t *fp);


/* IO handling */
_CY_API extern cy_error_t cy_fp_import( const uint8_t *in, size_t fp_t8, cy_fp_t *out );
_CY_API extern cy_error_t cy_fp_from_bn( const cy_bn_t *in, cy_fp_t *out );

_CY_API extern cy_error_t cy_fp_export(const cy_fp_t *in, uint8_t *out , size_t t8_out);
_CY_API extern cy_error_t cy_to_fp( uint8_t *in, size_t fp_t8,  DATA_FORMAT how, cy_fp_t *out );
_CY_API extern cy_error_t cy_from_fp( uint8_t *in, size_t fp_t8,  DATA_FORMAT how, cy_fp_t *out );
_CY_API extern cy_error_t cy_fp_set_zero(cy_fp_t *out);
_CY_API  extern cy_error_t cy_fp_set_one(cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_copy(const cy_fp_t *in, cy_fp_t *out);


/* Arithmetic functions */
_CY_API extern cy_error_t cy_fp_add(  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
_CY_API extern cy_error_t cy_fp_sub( cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
_CY_API extern cy_error_t cy_fp_neg( cy_fp_t *a,  cy_fp_t *out);

_CY_API extern cy_error_t cy_fp_inc(cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_dec(cy_fp_t *in, cy_fp_t *out);
//_CY_API extern cy_error_t cy_fp_cmp(cy_fp_t *in1, cy_fp_t *in2, boolean_t *cmp);

_CY_API extern cy_error_t cy_fp_mul(  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
_CY_API extern cy_error_t cy_fp_inv( cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_sqr( const cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_sqrt( cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_pow( cy_fp_t *in, uint8_t *scalar, size_t scalar_t8, cy_fp_t *out); /* for library enabling Montgomery, you should use montgomery version*/

/* Comparizons */
_CY_API extern cy_error_t cy_fp_isone(const cy_fp_t *in, int *iszero);
_CY_API extern cy_error_t cy_fp_iszero(const cy_fp_t *in, int *iszero);
_CY_API extern cy_error_t cy_fp_eq_or_opp(cy_fp_t *in1, cy_fp_t *in2, int *eq_or_opp);

/* Montgomery representation handling */
_CY_API extern cy_error_t cy_fp_mont_import(const uint8_t *in, const size_t fp_t8, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_mont_export(const cy_fp_t *in, uint8_t *out , size_t t8_out);

_CY_API extern cy_error_t cy_fp_to_mont(cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_from_mont(cy_fp_t *in, cy_fp_t *out);
_CY_API extern cy_error_t cy_fp_mult_mont( cy_fp_t *a, cy_fp_t *b, cy_fp_t *r);
_CY_API extern cy_error_t cy_fp_pow_mont( cy_fp_t *in, uint8_t *scalar, size_t scalar_t8, cy_fp_t *out); /* for library enabling Montgomery, you should use montgomery version*/

#include "cy_gda_component.h"
_CY_API extern cy_error_t cy_fp_get_random(cy_gda_ctx_t *gda, cy_fp_t *out );

/*Access fields*/
size_t   get_fp_size_field(cy_fp_ctx_t *ps_ctx);
_CY_API extern cy_bn_t* cy_get_fp_montgomery_constant1(const cy_fp_ctx_t *ps_ctx); /* return NULL if not handled, or a pointer to the field */
_CY_API extern cy_bn_t* cy_get_fp_montgomery_constant2(const cy_fp_ctx_t *ps_ctx); /* return NULL if not handled, or a pointer to the field */
_CY_API extern cy_bn_t* cy_get_fp_montgomery_one(const cy_fp_ctx_t *ps_ctx);
#endif /* API_CY_FP_H_ */
