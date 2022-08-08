/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos_ec.c								   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib elliptic curves arithmetic              */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"
#include "cy_wrap_bolos_ec.h"
#include "cy_ec.h"
#include "cy_ec_const.h"


/*****************************************************************************/
/*	I. completing missing Speculos functions								 */
/*****************************************************************************/

 cx_err_t cy_ecdomain_parameter_bn	(	cx_curve_t 	cv, cx_curve_dom_param_t 	id, cx_bn_t 	out )
 {
	 cy_error_t error=CY_UNHANDLED_CURVE;

	 /* we only require the prime field p value here to wrap ec init*/
	 if(id!=CX_CURVE_PARAM_Field) return CY_INCOMPLETE_IMPLEMENTATION;

	 if(cv==CX_CURVE_SECP256K1){
		 CY_CHECK(sys_cx_bn_init(out, C_cx_secp256k1_p, SEC256K1_SIZE_u8));

	 }

	 if(cv==CX_CURVE_SECP384R1){
		 CY_CHECK(sys_cx_bn_init(out, C_cx_secp384r1_p, SEC384r1_SIZE_u8));

	 }

	 /*todo: complete with other bolos curves*/
		 /*
		 CX_CURVE_BLS12_381_G1
		 CX_CURVE_SECP384R1
		 */
	 end:
	 	return error;

 }



/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling			 */
/*****************************************************************************/

/*bolos argc input is 1 with argv[0]=curve_name*/
cy_error_t wrap_bolos_ec_init(ec_ctx_t *ec_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]){

	cy_error_t error=CY_KO;
	cx_curve_t curve=argv[0];

	unsigned char argv_curve={"",0};

	if(cx_ecfp_get_domain(curve)==NULL) return CY_UNHANDLED_CURVE;

	ec_ctx->offset=0;

	ec_ctx->Shared_Memory[0]=(uint8_t) *argv[0];
	ec_ctx->Shared_Memory[1]= 0;
	ec_ctx->Shared_Memory[2]= 0;
	ec_ctx->Shared_Memory[3]= 0;

	ec_ctx->offset+=sizeof(uint8_t);

   CY_CHECK( cy_ecdomain_parameter_bn(	curve,	CX_CURVE_PARAM_Field 	, 	bn_p	));

	wrap_bolos_fp_init(ec_ctx->modular_p, pu8_Mem ,  t8_Memory,	2, argv_curve);

	end:
	return error;

}

cy_error_t wrap_bolos_ec_add( cy_ecpoint_t *a, cy_fp_t *b, cy_fp_t *out){
 cy_error_t error=CY_KO;
 fp_ctx_t *ctx =a->ctx;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;

	 goto end;
 }

 CX_CHECK(sys_cx_bn_mod_add(*(out->bn),*(a->bn),*(b->bn), *((cx_bn_t *) ctx->modular)));

 error=CY_OK;

 end:
  return error;
}





