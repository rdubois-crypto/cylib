/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_test_wrap_fp2.c
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

#include "cy_test_common_tools.h"

/* define here one of the burritos only if this file is defined as an autosufficient main*/
#ifndef _TEST_ALL
#define _BOLOS_BURRITOS
#endif

//#define _BOLOS_BURRITOS
//#define _LIB256K1_BURRITOS


#include "innovation/cy_ec_const.h"
#include "innovation/cy_errors.h"
#include "innovation/cy_fp.h"

#include "innovation/cy_quad.h"
#include "innovation/cy_fp2.h"


static int test_fp2_crypto_parameters(const uint8_t *argv[], int argc, char *name, uint8_t *Ramp, size_t sizeRam)
{
  fp_ctx_t ctx;

  cy_error_t error = CY_OK;


  size_t parameters_t8=(size_t) (argv[0][0]);

  /* The shared ram between program and library*/
  debug_printf("\n @RAMP=%x\n sizeRamp=%x", (unsigned int)Ramp,(int)sizeRam);
  Print_RAMp(Ramp, sizeRam);

  /* Initializing the fp unit*/
  CY_CHECK(cy_fp2_init(&ctx, Ramp, sizeRam, argc, argv));
  printf("\n %s  : %s:", ctx.libname, name);
  debug_printf("\n After init");
  Print_RAMp(Ramp, sizeRam);

  //test_fp2_add(&ctx, Ramp, sizeRam);


  /* Closing the fp unit*/
  debug_printf("\n uninit");

  CY_CHECK(cy_fp2_uninit(&ctx, Ramp, sizeRam));
  Print_RAMp(Ramp, sizeRam);

end:
  return error;
}

/* note that only stark and bls12 support quadratic extensions with X2+1=0 */
static cy_error_t test_fp2_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;
	size_t nb_supported=sizeof(C_cx_allCurves)/sizeof(cx_testvec_weierstrass_t *);

	printf("\n\n /************************ Test Quadratic Extension Unit:");

	#ifdef _FP2_UNIT_DISABLED
	printf("\n <not implemented with this FP Unit> : skipped : OK");
	return CY_OK;
 	#endif

	printf("\n nb supported modules=%d", nb_supported);

	// const uint8_t *argv_bls381[] = { secp384r1_t8, C_cx_BLS12_381_G1_p };
	 //const uint8_t *argv_sec384[] = { secp384r1_t8, C_cx_secp384r1_p };
	 //C_cx_secp384r1->argv,
    const uint8_t *argv_gen[]={NULL, NULL};

    if(nb_supported>4) nb_supported=4;

	for(i=0;i<nb_supported;i++){

	    argv_gen[0]=C_cx_allCurves[i]->t8_size;
	    argv_gen[1]=C_cx_allCurves[i]->p;

	 CY_CHECK(test_fp2_crypto_parameters(argv_gen, 2, C_cx_allCurves[i]->curve_name, Ramp, Ramp_t8));
	}

	end:
	  return error;
}

