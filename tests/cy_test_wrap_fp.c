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
/* DESCRIPTION: testing wrappers for modular arithmetic */
/**********************************************************************************/

#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <stdbool.h>
#include <stdio.h>


#include "cy_test_common_tools.h"

/* define one of the burritos only if defined as an autosufficient main*/
#ifndef _TEST_ALL
#define _BOLOS_BURRITOS
#endif

//#define _BOLOS_BURRITOS
#ifdef _BOLOS_BURRITOS
#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#include "../../../src/innovation/cy_wrap_fp_bolos.h"

#endif


//#define _LIB256K1_BURRITOS
#ifdef _LIB256K1_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_lib256k1.h"
#endif

#include "innovation/cy_ec_const.h"
#include "innovation/cy_errors.h"

#include "innovation/cy_fp.h"

#define _MAX_SIZE_TESTED_FP_T8 48





/* size coded on 64 bits lsb*/


/* test vector from cy_multmontgomery.sage for ?*/

uint8_t mont_aR[] = { 50,  158, 253, 123, 63,  48,  216, 178, 194, 71,  108,
                      128, 69,  6,   72,  77,  252, 209, 80,  67,  106, 12,
                      191, 135, 26,  215, 161, 199, 142, 42,  195, 69 };
uint8_t mont_bR[] = { 73,  229, 65,  191, 148, 133, 121, 235, 78,  146, 235,
                      245, 232, 240, 248, 15,  24,  76,  109, 176, 77,  248,
                      103, 0,   19,  221, 232, 109, 23,  205, 242, 150 };
uint8_t mont_expected[] = { 243, 209, 236, 247, 222, 138, 146, 149,
                            110, 224, 82,  66,  185, 230, 52,  45,
                            138, 75,  232, 241, 150, 92,  54,  147,
                            104, 110, 11,  160, 103, 145, 24,  77 };

uint8_t mod_a[] = { 172, 55,  63,  63,  189, 248, 237, 255, 103, 8,  115, 206, 227, 149, 169, 53,
			        50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125,
					50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125
};

uint8_t mod_b[] = { 193, 85,  77, 181, 65,  190, 220, 240, 100, 143, 53,
                    144, 77,  82, 124, 182, 73,  229, 65,  191, 148, 133,
                    121, 235, 78, 146, 235, 245, 39,  155, 167, 120,
					50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125};

uint8_t mod_expected[] = { 57,  165, 57,  230, 138, 185, 240, 26,
                           38,  190, 226, 197, 137, 238, 151, 189,
                           243, 209, 236, 247, 222, 138, 146, 149,
                           110, 224, 82,  66,  128, 64,  249, 107 };


/* test reciprocity of import and export*/
/* TODO*/

/* We test simultaneously mul, add, sub and pow using a little fermat inversion loop checking that a^(p-2)-a^-1==0 with random input*/
/* input : allocated fp (a,b,r)*/
/* TODO*/
int test_InversionFermatLoop(cy_fp_t *a, cy_fp_t *b, cy_fp_t *r)
{
  cy_error_t error = CY_OK;

  /* TODO*/
  /* We test simultaneously mul and inv by testing reciprocity */
  /* TODO*/

  end:
	  return error;
}


static cy_error_t test_fp_montgomery(fp_ctx_t *ctx)
{
	cy_error_t error = CY_OK;


	end:
		  return error;
}

static cy_error_t test_fp_add(fp_ctx_t *ctx, uint8_t *Ramp, size_t sizeRam)
{
	 cy_error_t error = CY_OK;
	 cy_fp_t fp_a, fp_b, fp_apowp, fp_r;
	 uint8_t exported[_MAX_SIZE_TESTED_FP_T8];
	  uint8_t test_a[_MAX_SIZE_TESTED_FP_T8];
	  uint8_t test_b[_MAX_SIZE_TESTED_FP_T8];
	  size_t parameters_t8= ctx->t8_modular;

	  debug_printf("\n-Alloc and Import");

	  //printf("\n here offset=%d", (int) ctx.offset);
	  CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_a));
	  debug_printf("\n After 1 Alloc");
	   Print_RAMp(Ramp, sizeRam);

	  //printf("\n here offset=%d", (int) ctx.offset);
	  CY_CHECK(cy_fp_import(mod_a, parameters_t8, &fp_a));
	  debug_printf("\n After 1 import");
	   Print_RAMp(Ramp, sizeRam);

	  CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_b));
	  CY_CHECK(cy_fp_import(mod_b, parameters_t8, &fp_b));

	  CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_r));

	  debug_printf("\n-Add and export");
	  CY_CHECK(cy_fp_add(&fp_a, &fp_b, &fp_r));
	  debug_printf("\n after add");
	  Print_RAMp(Ramp, sizeRam);


	  CY_CHECK(cy_fp_export(&fp_r, exported, parameters_t8));

	  debut_print_MsbString(exported, parameters_t8, "\n Add result:\n");



	  CY_CHECK(cy_fp_free(&fp_a));
	  CY_CHECK(cy_fp_free(&fp_b));
	  CY_CHECK(cy_fp_free(&fp_r));
	  debug_printf("\n after free");
	  Print_RAMp(Ramp, sizeRam);


	end: // printf("\n end crypto");
	  if (error == CY_OK)
	    printf(" OK !");
	  else
	    printf(" KO\n");
		  return error;
}


int test_crypto_parameters(const uint8_t *argv[], int argc, char *name, uint8_t *Ramp, size_t sizeRam)
{
  fp_ctx_t ctx;

  cy_error_t error = CY_OK;


  size_t parameters_t8=(size_t) (argv[0][0]);

  /* The shared ram between program and library*/
  debug_printf("\n @RAMP=%x\n sizeRamp=%x", (unsigned int)Ramp,(int)sizeRam);
  Print_RAMp(Ramp, sizeRam);

  /* Initializing the fp unit*/
  CY_CHECK(cy_fp_init(&ctx, Ramp, sizeRam, argc, argv));
  printf("\n %s  : %s:", ctx.libname, name);
  debug_printf("\n After init");
  Print_RAMp(Ramp, sizeRam);

  test_fp_add(&ctx, Ramp, sizeRam);


  /* Closing the fp unit*/
  debug_printf("\n uninit");

  CY_CHECK(cy_fp_uninit(&ctx, Ramp, sizeRam));
  Print_RAMp(Ramp, sizeRam);

end:
  return error;
}


static cy_error_t test_fp_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;
	size_t nb_supported=sizeof(C_cx_allCurves)/sizeof(cx_testvec_weierstrass_t *);

	printf("\n/************************ Test Fp Unit:");

	printf("\n nb supported curves=%d", nb_supported);

	// const uint8_t *argv_bls381[] = { secp384r1_t8, C_cx_BLS12_381_G1_p };
	 //const uint8_t *argv_sec384[] = { secp384r1_t8, C_cx_secp384r1_p };
	 //C_cx_secp384r1->argv,
    const uint8_t *argv_gen[]={NULL, NULL};

    if(nb_supported>2) nb_supported=2;

	for(i=0;i<nb_supported;i++){

	    argv_gen[0]=C_cx_allCurves[i]->t8_size;
	    argv_gen[1]=C_cx_allCurves[i]->p;

	 CY_CHECK(test_crypto_parameters(argv_gen, 2, C_cx_allCurves[i]->curve_name, Ramp, Ramp_t8));
	}

	end:
	  return error;
}



#ifndef _TEST_ALL
int main()
{

  uint8_t Ramp[_MAX_MEMORY];

  cy_error_t error = CY_OK;
  CY_CHECK(test_fp_unit(Ramp, _MAX_MEMORY));

end:
  printf("\n Overall results:");
  if (error == CY_OK)
    printf(" OK !\n");
  else
    printf(" KO\n");
  return (int)error;
}
#endif
