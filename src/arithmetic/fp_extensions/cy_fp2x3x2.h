/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_fp12.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: cy_lib NATIVE dodecaic field extensions for cy_fp12.h implementation using */
/* FP12=((FP2)^3)^2, implementation adapted from blst library  */
/* Fp modulus must have -1 as a square */
/**********************************************************************************/

#ifndef CY_FP2X3X2_H_
#define CY_FP2X3X2_H_


typedef cy_fp2_t cy_fp6_t[3];
typedef cy_fp6_t cy_fp12_t[2];

#define CY_FP12_LIBNAME "<CY_LIB FP12 Module>"


struct fp2x3x2_ctx_s {
  cy_flag_t is_initialized;
  char libname[T8_MAX_LIBNAME];
  size_t offset;

  size_t t8_modular_p;
  fp2_ctx_t ctx_quad;

  void *modular_p; /*pointer to modular context in Shared Memory*/
};

typedef struct fp2x3x2_ctx_s fp2x3x2_ctx_t;


#define fp12_ctx_t fp2x3x2_ctx_t


/* TODO: revise initializer*/
cy_error_t cy_fp2x3x2_init(fp12_ctx_t *ps_ctx_fp12, uint8_t *pu8_Mem,
                       const size_t t8_Memory, const int argc,
                       const uint8_t *argv[]);


#define fp12_ctx_t fp2x3x2_ctx_t
typedef cy_fp2_t cy_fp6_t[3];
typedef cy_fp6_t cy_fp12_t[2];
#define cy_fp12_alloc(a, out) cy_fp2x3x2_alloc(a, out)

#define cy_fp12_init(ctx, pMem, t8_mem,  argc,argv) (cy_fp2x3x2_init(ctx, pMem, t8_mem,  argc,argv))
#define cy_fp12_uninit(ctx, pMem, t8_mem) (cy_fp2x3x2_uninit(ctx, pMem, t8_mem))
#define cy_fp12_inv(in, out) (cy_fp2x3x2_inv(in, out))
#define cy_fp12set_zero(io) (cy_fp2x3x2_set_zero(io))
#define cy_fp12_cyclotomic_sqr(a,b) cy_fp2x3x2_cyclotomic_sqr(a,b)
#define cy_fp12_sqr(a,b) cy_fp2x3x2_sqr(a,b)
#endif /* SRC_INNOVATION_CY_FP2X3X2_H_ */
