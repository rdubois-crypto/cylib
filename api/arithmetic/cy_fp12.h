/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_fp12.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Dodecaic extensions Integers API */
/**********************************************************************************/

#ifndef _CY_fp12_H
#define _CY_fp12_H




_CY_API extern cy_error_t cy_fp12_init(fp12_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory, const int argc,
                              const uint8_t *argv[]);

_CY_API extern cy_error_t cy_fp12_alloc(fp12_ctx_t *ps_ctx, cy_fp12_t *out);

_CY_API extern cy_error_t cy_fp2x6_alloc(fp2_ctx_t *ctx_fp2, cy_fp12_t *a);

_CY_API extern cy_error_t cy_fp12_import(uint8_t *in, size_t fp12_t8,
                                cy_fp12_t *out);
_CY_API extern cy_error_t  cy_fp12_copy( const cy_fp12_t *in,
        cy_fp12_t *out);

_CY_API extern cy_error_t cy_fp12_set_zero(cy_fp12_t *io);

_CY_API extern cy_error_t cy_fp12_add(cy_fp12_t *a, cy_fp12_t *b,
                             cy_fp12_t *r);

_CY_API extern cy_error_t cy_fp12_sub(cy_fp12_t *a, cy_fp12_t *b,
                             cy_fp12_t *r);

_CY_API extern cy_error_t cy_fp12_mul(const cy_fp12_t *a, const cy_fp12_t *b,
                             cy_fp12_t *r);

_CY_API extern cy_error_t cy_fp2x3x2_inv(const cy_fp12_t *in, cy_fp12_t *out);

_CY_API extern cy_error_t cy_fp12_conjugate( cy_fp12_t *out);

_CY_API extern cy_error_t cy_fp12_cyclotomic_sqr(const cy_fp12_t *in, cy_fp12_t *out);

_CY_API extern cy_error_t cy_fp12_sqr(const cy_fp12_t *in, cy_fp12_t *out);


_CY_API extern cy_error_t cy_fp12_iseq(const cy_fp12_t *in1, const cy_fp12_t *in2, boolean_t *res);

_CY_API extern cy_error_t cy_fp6_alloc(fp2_ctx_t *ctx_fp2, cy_fp6_t *a);
_CY_API extern cy_error_t cy_fp6_neg( const cy_fp6_t a, cy_fp6_t ret);

_CY_API extern cy_error_t cy_fp12_free(cy_fp12_t *fp12);

_CY_API extern cy_error_t cy_fp12_uninit(fp12_ctx_t *ctx, uint8_t *pu8_Mem,
                                const size_t t8_Memory);

#endif
