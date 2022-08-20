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
/* DESCRIPTION: libECC wrappers for cy_lib prime fied arithmetic */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "cy_def.h"
#include "cy_errors.h"

#include "cy_wrap_fp_libECC.h"

#include "external/libECC/words/words.h"
#include "external/libECC/nn/nn.h"
#include "external/libECC/fp/fp.h"


#include "cy_fp.h"


cy_error_t wrap_libECC_fp_init(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
        const size_t t8_Memory, const int argc,
        const uint8_t *argv[])
{
	cy_error_t error=CY_OK;
	fp_ctx ctx;

	size_t i;
	if(argc!=2)
		return CY_KO;

	ps_ctx->t8_modular = word32_from_be(argv[0]);

	 if (t8_Memory > _MAX_MEMORY) {
	    error = CY_KO;
	    goto end;
	  }

	nn p;
	CY_CHECK(nn_init(&p, (u16) ps_ctx->t8_modular));
	CY_CHECK(fp_ctx_init_from_p(&ctx, &p));

	for(i=0;i<sizeof(fp_ctx);i++)
		pu8_Mem[i]= ((uint8_t *) (&ctx))[i];

	ps_ctx->modular=pu8_Mem;
	ps_ctx->offset=sizeof(fp_ctx);
	ps_ctx->montgomery_ctx=NULL;
	strcpy(ps_ctx->libname,LIBECC_FP_LIBNAME);

	end:
  	  return error;
}
