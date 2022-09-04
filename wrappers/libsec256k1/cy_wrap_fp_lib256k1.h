/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_fp_lib256k1.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: lib256k1 wrappers for cy_lib, header file */
/**********************************************************************************/

#ifndef SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_
#define SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_

/* byte size of a lib256k1 modular field element*/
#define _LIB256K1_FE_T8 (32)

#define LIB256K1_LIBNAME "<libsecp256k1:Fp Module>"

#include "external/libsec256k1/libsec256k1_field_10x26.h"

#define cy_bn_t secp256k1_fe

#define cy_fp_init(a, b, c, d, e) (wrap_lib256k1_fp_init(a, b, c, d, e))
#define cy_fp_uninit(i_cryptolib, ram, ramsize) wrap_lib256k1_fp_uninit(i_cryptolib, ram, ramsize)

#define cy_fp_import(in, size, out) (wrap_lib256k1_fp_import(in, size, out))
#define cy_fp_set_zero(a) (wrap_lib256k1_fp_set_zero(a))

#define cy_fp_export(in, size, out) (wrap_lib256k1_fp_export(in, size, out))
#define cy_fp_copy(in,  out) (wrap_lib256k1_fp_copy(in, out))
#define cy_fp_add(in1, in2, out) (wrap_lib256k1_fp_add(in1, in2, out))
#define cy_fp_sub(in1, in2, out) (wrap_lib256k1_fp_sub(in1, in2, out))

/* montgomery not required with solinas reduction*/
#define cy_fp_mont_import(in1, size, out) (wrap_lib256k1_fp_import(in1, size, out))

#define cy_fp_mul(in1, in2, out) (wrap_lib256k1_fp_mul(in1, in2, out))
#define cy_fp_mult_mont(in1, in2, out) (wrap_lib256k1_fp_mult_mont(in1, in2, out))
#define cy_fp_sqr(a,b) (wrap_lib256k1_fp_sqr(a, b))

#define cy_fp_inv(in1, out) (wrap_lib256k1_fp_inv(in1, out))

#define  cy_fp_neg( in, out) (wrap_lib256k1_fp_neg(in, out))
#define cy_fp_alloc(c, s, r) (wrap_lib256k1_fp_alloc(c, s, r))
#define cy_fp_free(fp) (wrap_lib256k1_fp_free(fp))

#define get_fp_montgomery_constant1(p_ctx_fp) (wrap_lib256k1_get_fp_montgomery_constant1(p_ctx_fp))

#define cy_bn_export(in, out, size)

#endif /* SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_ */
