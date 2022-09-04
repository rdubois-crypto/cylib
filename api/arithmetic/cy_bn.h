/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_fp.h										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Multiprecision Integers API                                       */
/**********************************************************************************/

#ifndef API_CY_BN_H_
#define API_CY_BN_H_

#include "cy_def.h"
#include "cy_errors.h"
#include "cy_memory_unit.h"

_CY_API extern cy_error_t cy_bn_export( const cy_bn_t *in,    size_t bn_t8, uint8_t *out );

/* IO handling */
_CY_API extern cy_error_t cy_bn_import( const uint8_t *in, size_t bn_t8, cy_bn_t *out );
_CY_API extern cy_error_t cy_bn_from_bn( const uint8_t *in, size_t bn_t8, cy_bn_t *out );

_CY_API extern cy_error_t cy_bn_export(const cy_bn_t *in, uint8_t *out , size_t t8_out);
_CY_API extern cy_error_t cy_bn_set_zero(cy_bn_t *out);
_CY_API  extern cy_error_t cy_bn_set_one(cy_bn_t *out);
_CY_API extern cy_error_t cy_bn_copy(const cy_bn_t *in, cy_bn_t *out);


/* Arithmetic functions */
_CY_API extern cy_error_t cy_bn_add(  cy_bn_t *a, cy_bn_t *b, cy_bn_t *r);
_CY_API extern cy_error_t cy_bn_sub( cy_bn_t *a, cy_bn_t *b, cy_bn_t *r);
_CY_API extern cy_error_t cy_bn_neg( cy_bn_t *a,  cy_bn_t *out);

_CY_API extern cy_error_t cy_bn_inc(cy_bn_t *in, cy_bn_t *out);
_CY_API extern cy_error_t cy_bn_dec(cy_bn_t *in, cy_bn_t *out);
//_CY_API extern cy_error_t cy_bn_cmp(cy_bn_t *in1, cy_bn_t *in2, boolean_t *cmp);

_CY_API extern cy_error_t cy_bn_mul(  cy_bn_t *a, cy_bn_t *b, cy_bn_t *r);
_CY_API extern cy_error_t cy_bn_inv( cy_bn_t *in, cy_bn_t *out);
_CY_API extern cy_error_t cy_bn_sqr( const cy_bn_t *in, cy_bn_t *out);
_CY_API extern cy_error_t cy_bn_sqrt( cy_bn_t *in, cy_bn_t *out);
_CY_API extern cy_error_t cy_bn_pow( cy_bn_t *in, uint8_t *scalar, size_t scalar_t8, cy_bn_t *out); /* for library enabling Montgomery, you should use montgomery version*/

/* Comparizons */
_CY_API extern cy_error_t cy_bn_isone(const cy_bn_t *in, int *iszero);
_CY_API extern cy_error_t cy_bn_iszero(const cy_bn_t *in, int *iszero);
_CY_API extern cy_error_t cy_bn_eq_or_opp(cy_bn_t *in1, cy_bn_t *in2, int *eq_or_opp);



#endif
