/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_fp2.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Quadratic extensions Integers API */
/**********************************************************************************/

#ifndef _CY_FP2_H
#define _CY_FP2_H


#define LIBFP2_LIBNAME "<CY_LIB :Fp2 Extension Module>"

_CY_API extern cy_error_t cy_fp2_init(fp2_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory, const int argc,
                              const uint8_t *argv[]);

_CY_API extern cy_error_t cy_fp2_uninit(fp2_ctx_t *ctx, uint8_t *pu8_Mem,
                                const size_t t8_Memory);


_CY_API extern cy_error_t cy_fp2_alloc(fp2_ctx_t *ps_ctx, cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_import(uint8_t *in, size_t fp2_t8,
                                cy_fp2_t *out);

_CY_API extern cy_error_t cy_fp2_export(const cy_fp2_t *in, uint8_t *out , size_t t8_out);

_CY_API extern cy_error_t cy_fp2_copy(const  cy_fp2_t *a,  cy_fp2_t *out);


_CY_API extern cy_error_t cy_fp2_set_zero(cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_add(const cy_fp2_t *a, const cy_fp2_t *b,
                             cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_sub( const cy_fp2_t *a, const cy_fp2_t *b,
                             cy_fp2_t *out);

_CY_API extern cy_error_t cy_fp2_mul( const cy_fp2_t *a, const cy_fp2_t *b,
                             cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_sqr( const cy_fp2_t *in, cy_fp2_t *out_sqr);


_CY_API extern cy_error_t cy_fp2_inv(const  cy_fp2_t *a,  cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_neg(const  cy_fp2_t *a,  cy_fp2_t *out);
_CY_API extern cy_error_t cy_fp2_free(cy_fp2_t *fp2);

#endif
