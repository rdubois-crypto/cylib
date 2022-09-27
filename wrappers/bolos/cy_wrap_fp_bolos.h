/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file */
/**********************************************************************************/

#ifndef CY_WRAP_FP_BOLOS_H_
#define CY_WRAP_FP_BOLOS_H_

#define BOLOS_FP_LIBNAME "<lib_bolos_v2:Fp Module>"

//#define _MAX_MEMORY 256 /* max size of shared memory*/

#define _MAPPING_START (0)
#define _CX_BN_P       (1)

#define cy_bn_t cx_bn_t

/* while not specified, one can check that the montgomery representation used by
 * bolos is a 128 bits multiplier.
 */
struct cy_bn_mont_ctx_s {
  cy_bn_t n;
  //cy_bn_t h2; /* 2^2*bitsizeof(p) mod p */
  /* -p^-1 mod 2^(bitsizeof(word_t)) */
  cy_bn_t mpinv;
  cy_bn_t h; /* 2^bitsizeof(p) mod p, it is also 1 in montgo rep */
  cy_bn_t h2; /* 2^bitsizeof(p) mod p, it is also 1 in montgo rep */

};


typedef struct cy_bn_mont_ctx_s cy_bn_mont_ctx_t;

#define cy_fp_init(a, b, c, d, e) (wrap_bolos_fp_init(a, b, c, d, e))
#define cy_fp_uninit(ctx, mem, size) (wrap_bolos_fp_uninit(ctx, mem, size))

#define cy_fp_alloc(c, s, r) (wrap_bolos_fp_alloc(c, s, r))
#define cy_fp_free(fp) (wrap_bolos_fp_free(fp))

#define cy_fp_import(in, size, out) (wrap_bolos_fp_import(in, size, out))
#define cy_fp_export(in, out, size) (wrap_bolos_fp_export(in,  out, size))
#define cy_fp_set_zero(out) wrap_bolos_fp_set_zero(out)
#define cy_fp_add(a, b, r) (wrap_bolos_fp_add(a, b, r))
#define cy_fp_sub(a, b, r) (wrap_bolos_fp_sub(a, b, r))
#define  cy_fp_neg( in, out) (wrap_bolos_fp_neg(in, out))
#define  cy_fp_cmp( in1, in2, out) (in1, in2, out))
#define cy_fp_set_zero(out) wrap_bolos_fp_set_zero(out)

#define cy_fp_mul(a, b, r) (wrap_bolos_fp_mul(a, b, r))
#define cy_fp_sqr(in, out) (wrap_bolos_fp_sqr(in,  out))

#define cy_fp_mult_mont(a, b, r) (wrap_bolos_fp_mult_mont(a,b,r))
#define cy_fp_inv(a, r) (wrap_bolos_fp_inv(a, r))
#define cy_fp_copy(in, out) (wrap_bolos_fp_copy(in, out))

#define cy_fp_get_random(gda,out) wrap_bolos_fp_get_random(gda,out)

//#define cy_fp_inv(a, r) CY_UNIMPLEMENTED_F(cy_fp_inv, a,r)
/* montgomery */
#define cy_fp_to_mont(in, out) (wrap_bolos_to_mont(in, out))
#define cy_fp_from_mont(in, out) (wrap_bolos_from_mont(in, out))
#define cy_fp_mont_import(in, size, out) (wrap_bolos_mont_import(in, size, out))
#define cy_fp_mont_export(in, out, size) (wrap_bolos_mont_export(in, out,size))

/* Speculos is not emulating Montgomery functions, added here*/
#define _WITH_SPECULOS
#ifdef _WITH_SPECULOS
#define cx_mont_mul(r, a, b, c)           (cy_mont_mul(r, a, b, c))
#define cx_mont_init(ctx_out, n_in)       (cy_mont_init(ctx_out, n_in))
#define cx_mont_to_montgomery(out, in, c) (cy_mont_to_montgomery(out, in, c))
#define cx_mont_from_montgomery(out, in, c) (cy_mont_from_montgomery(out, in, c))
#define cx_bn_mod_invert_nprime(r,a,n) (sys_cx_bn_mod_invert_nprime(r,a,n))
#define cx_bn_mont_ctx_t                  cy_bn_mont_ctx_t
#define cx_mont_alloc(ctx, t8_length)     cy_mont_alloc(ctx, t8_length)


#else
#endif

/* Direct access functions */
#define cy_fp_from_bn(in,  out) (wrap_cy_fp_from_bn(in,  out))
#define cy_get_fp_montgomery_constant1(inout) (wrap_bolos_get_fp_montgomery_constant1(inout))
#define cy_get_fp_montgomery_constant2(inout) (wrap_bolos_get_fp_montgomery_constant2(inout))
#define cy_get_fp_montgomery_one(inout) (wrap_bolos_get_fp_montgomery_one(inout))


#endif /* SRC_INNOVATION_CY_WRAP_FP_BOLOS_H_ */
