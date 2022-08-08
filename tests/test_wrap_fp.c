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


#include "test_common_tools.h"

#define _BOLOS_BURRITOS
#ifdef _BOLOS_BURRITOS
#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#include "../../../src/innovation/cy_wrap_fp_bolos.h"

#endif


//#define _LIB256K1_BURRITOS
#ifdef _LIB256K1_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_lib256k1.h"
#endif


#include "innovation/cy_errors.h"

#include "innovation/cy_fp.h"

#define _MAX_SIZE_TESTED_FP_T8 48


static uint8_t BLS12_381_P[48] = {
  0x1a, 0x01, 0x11, 0xea, 0x39, 0x7f, 0xe6, 0x9a, 0x4b, 0x1b, 0xa7, 0xb6,
  0x43, 0x4b, 0xac, 0xd7, 0x64, 0x77, 0x4b, 0x84, 0xf3, 0x85, 0x12, 0xbf,
  0x67, 0x30, 0xd2, 0xa0, 0xf6, 0xb0, 0xf6, 0x24, 0x1e, 0xab, 0xff, 0xfe,
  0xb1, 0x53, 0xff, 0xff, 0xb9, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xaa, 0xab
};

static uint8_t const secp384r1_p[] = {
  // p:
  // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};

static uint8_t const secp384r1_t8[] = { 48, 0, 0, 0, 0, 0, 0, 0 };

const uint8_t *argv_bls381[] = { secp384r1_t8, BLS12_381_P };
const uint8_t *argv_sec384[] = { secp384r1_t8, secp384r1_p };

// p-1
static uint8_t secp384_r1_a[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe
};

// some number
static uint8_t secp384_r1_b[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xaa
};

static uint8_t secp384_r1_r[48] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static uint8_t C_cx_secp256k1_p[] = {
  // p:  0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x2f
};

/* size coded on 64 bits lsb*/
static uint8_t const secp256k1_t8[] = { 32, 0, 0, 0, 0, 0, 0, 0 };

const uint8_t *argv_sec256[] = { secp256k1_t8, C_cx_secp256k1_p };

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

uint8_t mod_a[] = { 172, 55,  63,  63,  189, 248, 237, 255, 103, 8,  115,
                    206, 227, 149, 169, 53,  50,  158, 253, 123, 63, 48,
                    216, 178, 194, 71,  108, 127, 152, 207, 6,   125 };
uint8_t mod_b[] = { 193, 85,  77, 181, 65,  190, 220, 240, 100, 143, 53,
                    144, 77,  82, 124, 182, 73,  229, 65,  191, 148, 133,
                    121, 235, 78, 146, 235, 245, 39,  155, 167, 120 };
uint8_t mod_expected[] = { 57,  165, 57,  230, 138, 185, 240, 26,
                           38,  190, 226, 197, 137, 238, 151, 189,
                           243, 209, 236, 247, 222, 138, 146, 149,
                           110, 224, 82,  66,  128, 64,  249, 107 };

struct cy_test_fp_s {
  size_t modular_t8;

  char *vector_name;
  uint8_t *a;
  uint8_t *b;
  uint8_t *r_add;
  uint8_t *r_mul;
  uint8_t *r_expo;

};

typedef struct cy_test_fp_s cy_test_fp_t;

cy_test_fp_t secp384_r1={
  384,

};

/* test reciprocity of import and export*/
/* input : allocated fp (a,b,r)*/
/* TODO*/
int test_InversionFermatLoop(cy_fp_t *a, cy_fp_t *b, cy_fp_t *r)
{
  cy_error_t error = CY_OK;


  end:
	  return error;
}

int test_crypto_parameters(const uint8_t *argv[], int argc, char *name, uint8_t *Ramp, size_t sizeRam)
{
  fp_ctx_t ctx;
  cy_fp_t fp_a, fp_b, fp_apowp, fp_r;
  cy_error_t error = CY_OK;
  uint8_t exported[_MAX_SIZE_TESTED_FP_T8];
  uint8_t test_a[_MAX_SIZE_TESTED_FP_T8];
  uint8_t test_b[_MAX_SIZE_TESTED_FP_T8];


  size_t parameters_t8=(size_t) (argv[0][0]);

  /* The shared ram between program and library*/
  debug_printf("\n @RAMP=%x\n sizeRamp=%x", (unsigned int)Ramp,(int)sizeRam);

  Print_RAMp(Ramp, sizeRam);

  CY_CHECK(cy_fp_init(&ctx, Ramp, sizeRam, argc, argv));
  printf("\n %s  : %s:", ctx.libname, name);
  debug_printf("\n After init");
  Print_RAMp(Ramp, sizeRam);

  debug_printf("\n-Alloc and Import");

  //printf("\n here offset=%d", (int) ctx.offset);
  CY_CHECK(cy_fp_alloc(&ctx, parameters_t8, &fp_a));
  debug_printf("\n After 1 Alloc");
   Print_RAMp(Ramp, sizeRam);

  //printf("\n here offset=%d", (int) ctx.offset);
  CY_CHECK(cy_fp_import(secp384_r1_a, parameters_t8, &fp_a));
  debug_printf("\n After 1 import");
   Print_RAMp(Ramp, sizeRam);

  CY_CHECK(cy_fp_alloc(&ctx, parameters_t8, &fp_b));
  CY_CHECK(cy_fp_import(secp384_r1_b, parameters_t8, &fp_b));

  CY_CHECK(cy_fp_alloc(&ctx, parameters_t8, &fp_r));

  debug_printf("\n-Add and export");
  CY_CHECK(cy_fp_add(&fp_a, &fp_b, &fp_r));
  debug_printf("\n after add");
  Print_RAMp(Ramp, sizeRam);


  CY_CHECK(cy_fp_export(&fp_r, exported, parameters_t8));

  debut_print_MsbString(exported, parameters_t8, "\n Add result:\n");

  /* We test simultaneously mul, add, sub and pow using a little fermat inversion loop checking that a^(p-2)-a^-1==0 with random input*/
  /* TODO*/
  /* We test simultaneously mul and inv by testing reciprocity */
  /* TODO*/


  debug_printf("\n After little fermat loop");
  Print_RAMp(Ramp, sizeRam);

  CY_CHECK(cy_fp_free(&fp_a));
  CY_CHECK(cy_fp_free(&fp_b));
  CY_CHECK(cy_fp_free(&fp_r));
  debug_printf("\n after free");
  Print_RAMp(Ramp, sizeRam);

  debug_printf("\n uninit");

  CY_CHECK(cy_fp_uninit(&ctx, Ramp, sizeRam));
  Print_RAMp(Ramp, sizeRam);

end:
  // printf("\n end crypto");
  if (error == CY_OK)
    printf(" OK !");
  else
    printf(" KO\n");

  return error;
}

static cy_error_t test_fp_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;

	printf("\n/************************ Test Fp Unit:");
#ifndef _LIB256K1_BURRITOS
	 CY_CHECK(test_crypto_parameters(argv_bls381, 2, "bls12_381", Ramp, Ramp_t8));
	 CY_CHECK(test_crypto_parameters(argv_sec384, 2, "secp384r1", Ramp, Ramp_t8));
#endif
	 CY_CHECK(test_crypto_parameters( argv_sec256, 2, "sec256k1",  Ramp, Ramp_t8));
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
