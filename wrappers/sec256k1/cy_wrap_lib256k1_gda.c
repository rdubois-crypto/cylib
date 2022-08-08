/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_gda.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "cy_def.h"
#include "cy_errors.h"
#include "cy_wrap_lib256k1_gda.h"

/*****************************************************************************/
/*	I. Temporary implementation
 */
/*****************************************************************************/
cy_error_t cy_rng_core(uint8_t *buffer, size_t len)
{
	 cy_error_t error=CY_OK;
	 size_t i;

	 /* with probability 1/2(8*len) this is indistinguishable from true randomness ;) */
	 for(i=0;i<len;i++)
		 buffer[i]=0xa1;

	 return error;
}

/*****************************************************************************/
/*	II. Wrapping lib256k1 calls to Common APIs  and Memory handling
 */
/*****************************************************************************/

cy_error_t cy_rng_lib256k1_init(cy_gda_ctx_t *io_ps_ctx, uint8_t *initializer,
                             size_t initializer_t8)
{

  io_ps_ctx->is_initialized = CY_LIB_INITIALIZED;
  UNUSED(initializer);
  UNUSED(initializer_t8);

  return CY_OK;
}

/* todo: plug lib256K1 deterministic rng*/
cy_error_t cy_rng_lib256k1_run(cy_gda_ctx_t *io_ps_ctx, uint8_t *buffer,
                            size_t len)
{
  cy_error_t error;

  if (io_ps_ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_INIT;
    goto end;
  }

  CY_CHECK(cy_rng_core(buffer, len));

  error = CY_OK;
end:
  return error;
}

cy_error_t cy_rng_lib256k1_uninit(cy_gda_ctx_t *io_ps_ctx)
{
  io_ps_ctx->is_initialized = 0;

  return CY_OK;
}

cy_gda_ctx_t lib256k1_gda_component = {

  NULL, /* no memory required by bolos gda*/

  0x256CA1, /**< Component identifier                                   */
  "lib256k1 GDA",

  /*Arithmetic functions*/
  (Fct_Gda_Init_t)cy_rng_lib256k1_init,
  (Fct_Gda_Run_t)cy_rng_lib256k1_run,
  (Fct_Gda_UnInit_t)cy_rng_lib256k1_uninit,

  0,

  NULL, /*internal state is private*/
  0x00, /*thus no size allocated*/

  NULL, /* no configuration */
  0x00  /*thus no size allocated*/
};
