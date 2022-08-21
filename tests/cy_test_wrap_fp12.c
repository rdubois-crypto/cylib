/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_test_wrap_fp12.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing wrappers for quadratic arithmetic */
/**********************************************************************************/

/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_test_wrap_fp12.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing wrappers for quadratic arithmetic */
/**********************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../../src/innovation/src/io/cy_io_common_tools.c"

/* define here one of the burritos only if this file is defined as an autosufficient main*/
#ifndef _TEST_ALL
#define _BOLOS_BURRITOS
#endif

//#define _BOLOS_BURRITOS
//#define _LIB256K1_BURRITOS


#include "cy_ec_const.h"
#include "cy_errors.h"
#include "cy_fp.h"
#include "cy_quad.h"
#include "cy_fp2.h"
#include "cy_fp2x3x2.h"
#include "cy_fp12.h"




static cy_error_t test_fp12_add(fp12_ctx_t *ctx, uint8_t *Ramp, size_t sizeRam)
{
	 cy_error_t error = CY_OK;
	 cy_fp12_t fp_a, fp_b, fp_apowp, fp_r;
	 uint8_t exported[_MAX_SIZE_TESTED_FP_T8];
	  uint8_t test_a[_MAX_SIZE_TESTED_FP_T8];
	  uint8_t test_b[_MAX_SIZE_TESTED_FP_T8];
	  size_t parameters_t8= ctx->ctx_quad.ctx.t8_modular;

	  debug_printf("\n-Alloc and Import");

#ifdef loopi

	  CY_CHECK(cy_fp12_alloc(ctx, parameters_t8, &fp_a));
	  debug_printf("\n After 1 Alloc");
	   debug_Print_RAMp(Ramp, sizeRam);

	  //printf("\n here offset=%d", (int) ctx.offset);
	  CY_CHECK(cy_fp12_import(quad_a, 2*parameters_t8, &fp_a));
	  debug_printf("\n After 1 import");
	   debug_Print_RAMp(Ramp, sizeRam);

	  CY_CHECK(cy_fp12_alloc(ctx, parameters_t8, &fp_b));
	  CY_CHECK(cy_fp12_import(mod_b, parameters_t8, &fp_b));

	  CY_CHECK(cy_fp12_alloc(ctx, parameters_t8, &fp_r));

	  debug_printf("\n-Add and export");
	//  CY_CHECK(cy_fp_add(&fp_a, &fp_b, &fp_r));
	  debug_printf("\n after add");
	  debug_Print_RAMp(Ramp, sizeRam);


	  CY_CHECK(cy_fp12_export(&fp_r, exported, parameters_t8));

	  debut_print_MsbString(exported, parameters_t8, "\n Add result:\n");



	  CY_CHECK(cy_fp12_free(&fp_r));
	  CY_CHECK(cy_fp12_free(&fp_b));


	  CY_CHECK(cy_fp12_free(&fp_a));
	  debug_printf("\n after free");
	  debug_Print_RAMp(Ramp, sizeRam);

#endif
	end: // printf("\n end crypto");
	  if (error == CY_OK)
	    printf(" OK !");
	  else
	    printf(" KO\n");
		  return error;
}

static int test_fp12_crypto_parameters(const uint8_t *argv[], int argc, char *name, uint8_t *Ramp, size_t sizeRam)
{
  fp12_ctx_t ctx;

  cy_error_t error = CY_OK;


  size_t parameters_t8=(size_t) (argv[0][0]);

  /* The shared ram between program and library*/
  debug_printf("\n @RAMP=%x\n sizeRamp=%x", (unsigned int)Ramp,(int)sizeRam);
  debug_Print_RAMp(Ramp, sizeRam);

  /* Initializing the fp unit*/
  CY_CHECK(cy_fp12_init(&ctx, Ramp, sizeRam, argc, argv));
  printf("\n %s  over %s: %s:", ctx.libname, ctx.ctx_quad.libname, name);

//  printf("\n %s  over %s: %s:", ctx.libname, ctx.ctx->libname, name);
  debug_printf("\n After init");
  debug_Print_RAMp(Ramp, sizeRam);

  test_fp12_add(&ctx, Ramp, sizeRam);


  /* Closing the fp unit*/
  debug_printf("\n uninit");

  CY_CHECK(cy_fp12_uninit(&ctx, Ramp, sizeRam));
  debug_Print_RAMp(Ramp, sizeRam);


end:
  return error;
}

/* note that only stark and bls12 support quadratic extensions with X2+1=0 */
static cy_error_t test_fp12_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;

	printf("\n\n /************************ Test Dodecaic (FP12) Extension Unit:");

	#ifdef _fp12_UNIT_DISABLED
	printf("\n <not implemented with this FP Unit> : skipped : OK");
	return CY_OK;
 	#endif

	printf("\n nb supported modules=%d", _QUADRATIC_FRIENDLY_END);

	// const uint8_t *argv_bls381[] = { secp384r1_t8, C_cx_BLS12_381_G1_p };
	 //const uint8_t *argv_sec384[] = { secp384r1_t8, C_cx_secp384r1_p };
	 //C_cx_secp384r1->argv,
    const uint8_t *argv_gen[]={NULL, NULL};



	for(i=0;i<_QUADRATIC_FRIENDLY_END;i++){

	    argv_gen[0]=C_cx_allCurves[i]->t8_size;
	    argv_gen[1]=C_cx_allCurves[i]->p;

	 CY_CHECK(test_fp12_crypto_parameters(argv_gen, 2, C_cx_allCurves[i]->curve_name, Ramp, Ramp_t8));
	}


	end:
	  return error;
}




