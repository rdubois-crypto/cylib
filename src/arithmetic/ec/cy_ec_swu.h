/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_ec.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: */
/* hashing of a Point over a Weierstrass curve from a digest using
 * SWU/Brier-Coron and all technique (alternative to Pedersen Hash)
 * https://eprint.iacr.org/2009/340.pdf*/
/**********************************************************************************/

#ifndef CY_EC_SWU_H_
#define CY_EC_SWU_H_


#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <stdbool.h>
#include <stdio.h>

#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"

/* Evaluation of X over elliptic curve of weierstrass form X^3+aX=b*/
cx_err_t cy_curve_eval_ysquare(cx_bn_t o_bn_y2, cx_bn_t i_bn_a, cx_bn_t i_bn_b,
                               cx_bn_t i_bn_x, cx_bn_t i_bn_p, size_t i_tu8_p);

/* hashing of a Point over a Weierstrass curve from a digest using
 * SWU/Brier-Coron and all technique (alternative to Pedersen Hash)
 * https://eprint.iacr.org/2009/340.pdf*/
cx_err_t cy_swu_hashpoint(cx_curve_t curve, cx_bn_t o_bn_x, cx_bn_t o_bn_y,
                          uint8_t *i_digest);



#endif /* SRC_INNOVATION_CY_EC_SWU_H_ */
