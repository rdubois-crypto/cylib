/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos_gda.h							   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file                            */
/**********************************************************************************/


#ifndef CY_WRAP_BOLOS_GDA_H_
#define CY_WRAP_BOLOS_GDA_H_

#include "cy_gda_component.h"
#include "bolos/cxlib.h"

#define cx_rng_no_throw (uint8_t *buffer, size_t len) cy_rng_no_throw (uint8_t *buffer, size_t len)

 cy_error_t cy_rng_bolos_init(gda_ctx_t *io_ps_ctx, uint8_t *initializer, size_t initializer_t8);
 cy_error_t cy_rng_bolos_run(gda_ctx_t *io_ps_ctx, uint8_t *initializer, size_t initializer_t8);
 cy_error_t cy_rng_bolos_uninit (gda_ctx_t *io_ps_ctx);

 gda_ctx_t bolos_gda_component={

		   NULL, /* no memory required by bolos gda*/

		   OxBB00110055,                   		/**< Component identifier                                   */
		   "Bolos GDA",

		   /*Arithmetic functions*/
		   (Fct_Gda_Init_t)					cy_rng_bolos_init,
		   (Fct_Gda_Run_t)  cy_rng_no_throw,
		   (Fct_Gda_UnInit_t)				cy_rng_bolos_uninit,

		   0,

		   NULL, /*internal state is private*/
		   0x00, /*thus no size allocated*/

		   NULL,/* no configuration */
		   0x00 /*thus no size allocated*/
		 };


#endif
