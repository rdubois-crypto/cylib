/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_ec.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib elliptic curves arithmetic */
/**********************************************************************************/

#ifndef CY_WRAP_EC_BOLOS_H_

#define _SEC256K1_ENABLED      // enabling bitcoin famous sec256k1 curve
#define _BLS12381_ENABLED      // enabling pairing friendly BLS12_381 curve
#define _SECP_ALL_ENABLED      // enabling all NIST secP curves
#define _BRAINPOOL_ALL_ENABLED // enabling all brainpool curves
#define _STARK_ENABLED         // enabling sparse stark friendly curve

#include "cy_ec_const.h"

/* important warning: the content of bolos cx_ec_point_t and emulated point
 * differ, their field shall only be accessed using the dedicated fonction,
 * considering x,y and z to be private or error will occur*/

#define coor_t cx_ecpoint_t;

/* Speculos is not emulating the following bolos functions*/
#define _WITH_SPECULOS
#ifdef _WITH_SPECULOS

struct cx_ecpoint_s {
  cy_bn_t x;
  cy_bn_t y;
  cy_bn_t z;
};

typedef struct cx_ecpoint_s cx_ecpoint_t;

#define cx_ecdomain_parameter_bn(cv, id, out)                                  \
  cy_ecdomain_parameter_bn(cv, id, out)

#endif

#define cx_bn_ec_add(a,b,out) cy_ecpoint_add(a,b,out)

#endif
