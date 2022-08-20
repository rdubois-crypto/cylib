/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project */
/* License: This software is licensed under a dual BSD and GPL v2 license */
/* See LICENSE file at the root folder of the project. */
/*                                                                                */
/* 																			      */
/* FILE: cy_pairing.h */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bls12 pairings, header file   */
/* Acknowledgment : this pairing is inspired from the blst library
/**********************************************************************************/



#ifndef _CY_PBC_H
#define _CY_PBC_H

#include "cy_def.h"
#include "cy_errors.h"

/* fp unit handling */
_CY_API extern  cy_error_t cy_pbc_init(pbc_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]);

_CY_API extern cy_error_t cy_pbc_uninit(pbc_ctx_t *ctx, uint8_t *pu8_Mem , const size_t t8_Memory);


_CY_API extern cy_error_t cy_pbc_pairing(cy_ecpoint_t *P1, cy_ecfpk_point_t *P2,
                    uint8_t *pairing, size_t size8_pairing);

_CY_API extern cy_error_t cy_pbc_pairing_asn1(cx_curve_t curve, unsigned char *P1, size_t P1_len,
                         unsigned char *P2, size_t P2_len,
                         const unsigned char *k, unsigned int k_len);


#endif /* _CY_PBC_H */
