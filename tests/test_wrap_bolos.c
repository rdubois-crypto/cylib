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

#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../../src/innovation/cy_wrap_fp_bolos.h"
#include "innovation/cy_def.h"

#include "innovation/cy_errors.h"

#include "innovation/cy_fp.h"

#include "innovation/cy_memory_unit.h"
#include "innovation/cy_cryptolib.h"
#include "innovation/cy_gda_component.h"
#include "innovation/cy_wrap_bolos_cryptolib.h"
#include "innovation/cy_wrap_bolos_gda.h"

static uint8_t const secp384r1_p[] = {
  // p:
  // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};

static uint8_t const secp384r1_t8[] = { 48, 0, 0, 0, 0, 0, 0, 0 };

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

static void Print_RAMp(uint8_t *Ramp)
{
  size_t i;
  printf("\n RAM:\n");
  for (i = 0; i < _MAX_MEMORY; i++) {
    printf("%02X", Ramp[i]);
    if ((i & 0xf) == 15)
      printf("\n");
  }
}

int test_mem_minimal_example()
{
  fp_ctx_t ctx;
  cy_fp_t fp_a, fp_b, fp_r;
  cy_error_t error = CY_OK;
  /* The shared ram between program and library*/
  uint8_t Ramp[_MAX_MEMORY];
  uint8_t *FpZone;

  cryptolib_ctx_t cryptolib_bolos;

  printf("\n @RAMP=%x", (unsigned int)Ramp);
  Print_RAMp(Ramp);


  printf("\n Starting lib, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_lib_init(&cryptolib_bolos, Ramp, sizeof(Ramp), NULL, 0));
  Print_RAMp(Ramp);

  printf("\n Allocating First Fp component space, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_mem_malloc(cryptolib_bolos.mem_unit, _MAX_MEMORY>>1, &FpZone));
  printf("\n returned malloc @=%x", (unsigned int) FpZone);

  Print_RAMp(Ramp);

  printf("\n Freeing First Fp component space, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_mem_free(cryptolib_bolos.mem_unit, FpZone, _MAX_MEMORY>>1));
  Print_RAMp(Ramp);

  printf("\n Closing lib, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_lib_uninit(&cryptolib_bolos));
  Print_RAMp(Ramp);

end:

  if (error == CY_OK)
    printf(" OK !\n");
  else
    printf(" KO\n");
  return error;
}

int main()
{
 return test_mem_minimal_example();

}
