/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_test_wrap_fp.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing wrappers for modular arithmetic */
/**********************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cy_configuration.h"
#include "cy_ec_const.h"
#include "cy_errors.h"
#include "cy_fp.h"

#include "cy_io_common_tools.h"


#define _MAX_SIZE_TESTED_FP_T8 48

/* expected precomputed constant n0 on sec256k1, obtained from sage */
expected_no_128_256k1[16]={0xbc, 0xb2, 0x23, 0xfe, 0xdc, 0x24, 0xa0, 0x59,
						   0xd8, 0x38, 0x09, 0x1d, 0xd2, 0x25, 0x35, 0x31};


/* size coded on 64 bits lsb*/


/* test vector from cy_multmontgomery.sage for ?*/

uint8_t mod_a[] = { 172, 55,  63,  63,  189, 248, 237, 255, 103, 8,  115, 206, 227, 149, 169, 53,
			        50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125,
					50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125
};

/*0xfcd150436a0cbf871ad7a1c7c0c9c1816ff63c0eebb26eb7b79b5dd46cf9eddd*/
/* expected montgomery 128 bits representant:0xfcd150436a0cbf871ad7a1c7c0c9c1816ff63c0eebb26eb7b79b5dd46cf9eddd*/

uint8_t mod_b[] = { 193, 85,  77, 181, 65,  190, 220, 240, 100, 143, 53,
                    144, 77,  82, 124, 182, 73,  229, 65,  191, 148, 133,
                    121, 235, 78, 146, 235, 245, 39,  155, 167, 120,
					50,  158, 253, 123, 63, 48,  216, 178, 194, 71,  108, 127, 152, 207, 6, 125};



/* test reciprocity of import and export*/
/* TODO*/

/* We test simultaneously mul, add, sub and pow using a little fermat inversion loop checking that a^(p-2)-a^-1==0 with random input*/
/* input : allocated fp (a,b,r)*/
/* TODO*/
int test_InversionFermatLoop(cy_fp_ctx_t *ctx)
{
  cy_error_t error = CY_OK;
  cy_fp_t fp_a, fp_b, fp_apowp, fp_r, fp_pm2, fp_am1, fp_temp;
  uint8_t two[1]={2};
  uint8_t one[1]={1};

  /* TODO*/
  /* We test simultaneously mul and inv by testing reciprocity */
  /* TODO*/
  	 uint8_t exported[_MAX_SIZE_TESTED_FP_T8];
  	  uint8_t test_a[_MAX_SIZE_TESTED_FP_T8];
  	  uint8_t test_b[_MAX_SIZE_TESTED_FP_T8];

  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_a));
  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_b));
  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_r));
  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_am1));
  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_temp));
  CY_CHECK(cy_fp_alloc(ctx, ctx->t8_modular, &fp_pm2));

  CY_CHECK(cy_fp_inv(&fp_a, &fp_am1));
  CY_CHECK(cy_fp_import(two, 1, &fp_pm2));
  CY_CHECK(cy_fp_sub(two, one, &fp_a));
  //CY_CHECK(cy_fp_pow(two, 1, &fp_a));



  end:
	  return error;
}


static cy_error_t test_fp_montgomery(cy_fp_ctx_t *ctx)
{
	cy_error_t error = CY_OK;
	cy_fp_t fp_a, fp_b, fp_aR, fp_bR;
	uint8_t res[SEC256K1_SIZE_u8];
	size_t parameters_t8= ctx->t8_modular;

	cy_fp_t fp_temp;
	CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_b));

	cy_bn_t* p_mont_h=cy_get_fp_montgomery_constant1(ctx);
	CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_temp));


	if((p_mont_h)==NULL)
	{
		printf("\n No Montgomery rep with this lib");
		return CY_OK;
	}

	printf("\n Test Montgomery on 256k1:");

	CY_CHECK(cy_fp_from_bn( p_mont_h, &fp_temp ));
	CY_CHECK(cy_io_fp_printMSB(&fp_temp, "\n R2="));

	p_mont_h=cy_get_fp_montgomery_constant2(ctx);
	CY_CHECK(cy_fp_from_bn( p_mont_h, &fp_temp ));
	CY_CHECK(cy_io_fp_printMSB(&fp_temp, "\n n0="));

	p_mont_h=cy_get_fp_montgomery_one(ctx);
	CY_CHECK(cy_fp_from_bn( p_mont_h, &fp_temp ));
	CY_CHECK(cy_io_fp_printMSB(&fp_temp, "\n R="));


	CY_CHECK(cy_fp_free(&fp_temp));
	CY_CHECK(cy_fp_free(&fp_b));


	CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_a));


	CY_CHECK(cy_fp_import(mod_a, parameters_t8, &fp_a));
	CY_CHECK(cy_fp_export( (&fp_a), res, parameters_t8 ));


	//CY_CHECK(cy_io_fp_printMSB(&fp_a, "\n import/export of fp_a:"));
	CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_b));
	CY_CHECK(cy_fp_import(mod_b, parameters_t8, &fp_b));


	CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_aR));
	CY_CHECK(cy_fp_mul(&fp_b, &fp_a, &fp_aR));

	CY_CHECK(cy_io_fp_printMSB(&fp_aR, "\n ---a*b= "));



	CY_CHECK(cy_fp_mont_import(mod_a, parameters_t8, &fp_aR));

	CY_CHECK(cy_io_fp_printMSB(&fp_a, "\n ---a= "));
	CY_CHECK(cy_io_fp_printMSB(&fp_aR, "\n ---aR= "));

	CY_CHECK(cy_fp_mult_mont(&fp_b, &fp_aR, &fp_aR));
	CY_CHECK(cy_io_fp_printMSB(&fp_aR, "\n ---Mulmont(aR*b)= "));

	//printf("n aR=%x", (unsigned int )*fp_aR.bn);

	//CY_CHECK(cy_bn_export( p_mont_h, res, parameters_t8 ));


	CY_CHECK(cy_fp_free(&fp_a));
	CY_CHECK(cy_fp_free(&fp_aR));
	CY_CHECK(cy_fp_free(&fp_b));




	end:
		  return error;
}

static cy_error_t test_fp_add(cy_fp_ctx_t *ctx, uint8_t *Ramp, size_t sizeRam)
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
	  debug_Print_RAMp(Ramp, sizeRam);

	  //printf("\n here offset=%d", (int) ctx.offset);
	  CY_CHECK(cy_fp_import(mod_a, parameters_t8, &fp_a));
	  debug_printf("\n After 1 import");
	  debug_Print_RAMp(Ramp, sizeRam);

	  CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_b));
	  CY_CHECK(cy_fp_import(mod_b, parameters_t8, &fp_b));

	  CY_CHECK(cy_fp_alloc(ctx, parameters_t8, &fp_r));

	  debug_printf("\n-Add and export");
	  CY_CHECK(cy_fp_add(&fp_a, &fp_b, &fp_r));
	  debug_printf("\n after add");
	  debug_Print_RAMp(Ramp, sizeRam);


	  CY_CHECK(cy_fp_export(&fp_r, exported, parameters_t8));

	  _debug(print_MsbString(exported, parameters_t8, "\n Add result:\n"));


	  CY_CHECK(cy_fp_free(&fp_a));
	  CY_CHECK(cy_fp_free(&fp_b));
	  CY_CHECK(cy_fp_free(&fp_r));
	  debug_printf("\n after free");
	  debug_Print_RAMp(Ramp, sizeRam);


	end: // printf("\n end crypto");
	  if (error == CY_OK)
	    printf(" OK !");
	  else
	    printf(" KO\n");
		  return error;
}


static int test_crypto_parameters( char *name, uint8_t *Ramp, size_t sizeRam, cx_testvec_weierstrass_t const *C_cy_allCurves)
{
  cy_fp_ctx_t ctx;

  cy_error_t error = CY_OK;

  const uint8_t *argv_gen[]={NULL, NULL};
  argv_gen[0]=C_cy_allCurves->t8_size;
  argv_gen[1]=C_cy_allCurves->p;

  size_t parameters_t8=(size_t) (argv_gen[0][0]);

  /* The shared ram between program and library*/
  debug_printf("\n @RAMP=%x\n sizeRamp=%x", (unsigned int)Ramp,(int)sizeRam);
  debug_Print_RAMp(Ramp, sizeRam);

  /* Initializing the fp unit*/
  CY_CHECK(cy_fp_init(&ctx, Ramp, sizeRam, 2, argv_gen));
  printf("\n %s  : %s:", ctx.libname, name);
  debug_printf("\n After init");
  debug_Print_RAMp(Ramp, sizeRam);

  test_fp_add(&ctx, Ramp, sizeRam);

  if(strcmp(name,"Curve sec256k1")==0)
	  test_fp_montgomery(&ctx);

  /* Closing the fp unit*/
  debug_printf("\n uninit");

  CY_CHECK(cy_fp_uninit(&ctx, Ramp, sizeRam));
  debug_Print_RAMp(Ramp, sizeRam);

end:
  return error;
}


static cy_error_t test_fp_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;
	size_t nb_supported=sizeof(C_cy_allCurves)/sizeof(cx_testvec_weierstrass_t *);

	printf("\n/************************ Test Fp Unit:");

	printf("\n nb supported curves=%d", nb_supported);

    const uint8_t *argv_gen[]={NULL, NULL};

    if(nb_supported>5) nb_supported=5;

	for(i=0;i<nb_supported;i++){

	    argv_gen[0]=C_cy_allCurves[i]->t8_size;
	    argv_gen[1]=C_cy_allCurves[i]->p;

	 CY_CHECK(test_crypto_parameters(C_cy_allCurves[i]->curve_name, Ramp, Ramp_t8, C_cy_allCurves[i]));
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
