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


/* important warning: the content of bolos cx_ec_point_t and emulated point
 * differ, their field shall only be accessed using the dedicated fonction,
 * considering x,y and z to be private or error will occur*/
#define BOLOS_EC_LIBNAME "<lib_bolos_v2:EC Module>"
#define coor_t cx_ecpoint_t;

#define cy_ec_init(ps_ctx, pu8_Mem,t8_Memory,  argc, argv) wrap_bolos_ec_init(ps_ctx, pu8_Mem,t8_Memory,  argc, argv)
#define cy_ec_uninit(ctx) wrap_bolos_ec_uninit(ctx)

#define cy_ec_import(xy, sx, P) wrap_bolos_ec_import(xy, sx, P)
#define cy_ec_import2( x, sx, y, sy, P) wrap_bolos_ec_import2( x, sx, y, sy, P)

#define cy_ec_alloc(ctx,P) wrap_ecpoint_alloc(ctx,P)
#define cy_ec_free(P) wrap_ecpoint_free(P)

#define cy_ec_isoncurve(P,booleen) wrap_bolos_isoncurve(P, booleen)
#define cy_ec_export(G, xy, sxy) wrap_bolos_ec_export(G, xy, sxy)
#define cy_ec_add(a,b,res) wrap_bolos_ec_add(a,b,res)
#define cy_ec_getX(a,b) wrap_bolos_getX(a,b)


#define cy_ec_scalarmult_fp(k,P,kP) wrap_bolos_ec_scalarmul_fp(k,P,kP)
#define cy_ec_get_generator(ctx, G) wrap_bolos_get_generator(ctx, G)


/* Speculos is not emulating the following bolos functions*/
#define _WITH_SPECULOS
#ifdef _WITH_SPECULOS

#define cy_inner_ec_t cx_ecpoint_t

#define cx_ecdomain_parameter_bn(cv, id, out)                                  \
  cy_ecdomain_parameter_bn(cv, id, out)

#endif
#define cx_ecpoint_add(a,b,c) cy_ecpoint_add(a,b,c)

#define cx_bn_ec_add(a,b,out) cy_ecpoint_add(a,b,out)
#define _MEM_EC_RESERVED 0xA5
#endif
