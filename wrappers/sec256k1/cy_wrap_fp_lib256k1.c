/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos.h								   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: lib256k1 wrappers for cy_lib	                                  */
/**********************************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "cy_errors.h"

#include "secp256k1.h"

#include "libsec256k1_field_10x26.h"
#include "libsec256k1_field_10x26_impl.h"
#include "libsec256k1_field_impl.h"
#include "cy_wrap_fp_lib256k1.h"
#include "cy_fp.h"


#define word32_from_be(a) (a[0]+(a[1]<<8)+(a[2]<<16)+(a[3]<<24) )



cy_error_t wrap_lib256k1_fp_init(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]){

	cy_error_t error=CY_KO;


	size_t i;
	size_t offset=0;

	if(argc!=2) {
		return CY_KO;
	}
	/*control memory limit*/
	if(t8_Memory>_MAX_MEMORY){
		error=CY_KO;
		goto end;
	}

	ps_ctx->Shared_Memory=pu8_Mem; /*set Memory*/


	strcpy(ps_ctx->libname, LIB256K1_LIBNAME);
	ps_ctx->is_initialized=CY_LIB_INITIALIZED;


	ps_ctx->t8_modular=word32_from_be(argv[0]);
	for(i=0;i<ps_ctx->t8_modular;i++) pu8_Mem[i+offset]=argv[1][i];
	offset+=i;

    ps_ctx->modular=(void*) ps_ctx->Shared_Memory;

    ps_ctx->is_initialized=CY_LIB_INITIALIZED;
	error=CY_OK;


	 UNUSED(secp256k1_fe_is_quad_var);
	 UNUSED(secp256k1_fe_inv_var);
	 UNUSED(secp256k1_fe_inv);
	 UNUSED(secp256k1_fe_to_storage);
	 UNUSED(secp256k1_fe_get_b32);
	 UNUSED (secp256k1_fe_set_b32);
	 UNUSED (secp256k1_fe_cmp_var);
	 UNUSED (secp256k1_fe_cmp_var);
	 UNUSED(secp256k1_fe_cmp_var);
	 UNUSED( secp256k1_fe_normalize_weak);
	 UNUSED( secp256k1_fe_get_bounds);

	end:
	return error;
}


cy_error_t wrap_lib256k1_fp_alloc(fp_ctx_t *ctx ,  size_t t8_r, cy_fp_t *r){
 cy_error_t error=CY_KO;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;
	 goto end;
 }

 UNUSED(t8_r);
 UNUSED(r);

 error=CY_OK;

 end:
  return error;
}

cy_error_t wrap_lib256k1_fp_import(  uint8_t *in, size_t t8_in, cy_fp_t *out){
	cy_error_t error=CY_KO;
	fp_ctx_t *ctx =out->ctx;

	size_t i;

	 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
		 error=CY_ERR_UNIT;
		 goto end;
	 }

	 if(t8_in!=32) return CY_ERR_LENGTH;
	 secp256k1_fe_storage cin;

	 for(i=0;i<32;i++){
		 (cin.n)[i]=word32_from_be( (in+(i<<3)));
	 }

	 (secp256k1_fe_from_storage(out->bn, &cin));

	 error=CY_OK;

	 end:
	  return error;

}


