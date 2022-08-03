/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos_gda.c								   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file                            */
/**********************************************************************************/

#include "cy_errors.h"
#include "cy_wrap_bolos_gda.h"

/*****************************************************************************/
/*	I. completing missing Speculos functions								 */
/*****************************************************************************/

cy_error_t cy_rng_no_throw (uint8_t *buffer, size_t len) {
	sys_cx_trng_get_random_data( (void *) buffer, len);

	return CY_OK;
}


/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling			 */
/*****************************************************************************/

cy_error_t cy_rng_bolos_init(gda_ctx_t *io_ps_ctx, uint8_t *initializer, size_t initializer_t8){

	io_ps_ctx->is_initialized=CY_LIB_INITIALIZED;
}

cy_error_t cy_rng_bolos_run(gda_ctx_t *io_ps_ctx, uint8_t *buffer, size_t len){
	cy_error_t error;

	if(io_ps_ctx->is_initialized!=CY_LIB_INITIALIZED) {
		 error=CY_ERR_INIT;
		 goto end;
	 }

	CY_CHECK(cy_rng_no_throw (buffer,  len));

	error=CY_OK;
	end:
		return error;
}

cy_error_t cy_rng_bolos_uninit (gda_ctx_t *io_ps_ctx){
	io_ps_ctx->is_initialized=0;
}

