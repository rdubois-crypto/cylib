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


/* TODO: revise initializer*/
cy_error_t cy_fp2x3x2_init(fp12_ctx_t *ps_ctx_fp12, uint8_t *pu8_Mem,
                       const size_t t8_Memory, const int argc,
                       const uint8_t *argv[]);


#define cy_fp12_init(ctx, pMem, t8_mem,  argc,argv) (cy_fp2x3x2_init(ctx, pMem, t8_mem,  argc,argv))



#endif /* SRC_INNOVATION_CY_FP2X3X2_H_ */
