/*
 * Copyright Ledger
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <stdbool.h>
#include <stdio.h>

#define _BOLOS_BURRITOS
#ifdef _BOLOS_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_bolos.h"
#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#endif


//#define _LIB256K1_BURRITOS
#ifdef _LIB256K1_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_lib256k1.h"
#endif


#include "innovation/cy_errors.h"

#include "innovation/cy_fp.h"

static uint8_t const secp384r1_p[] = {
  // p:
  // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};

static uint8_t const secp384r1_t8[] ={48, 0, 0, 0, 0, 0, 0, 0};

const uint8_t *argv_sec384[]={secp384r1_t8, secp384r1_p};

//p-1
static uint8_t secp384_r1_a[]={
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe
};

//some number
static uint8_t secp384_r1_b[]={
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xaa
};

static uint8_t secp384_r1_r[48]={
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static uint8_t C_cx_secp256k1_p[] = {
  // p:  0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfc, 0x2f
};

static uint8_t const secp256k1_t8[] ={32, 0, 0, 0, 0, 0, 0, 0};

const uint8_t *argv_sec256[]={secp256k1_t8, C_cx_secp256k1_p};


static void Print_RAMp(uint8_t *Ramp){
	size_t i;
	printf("\n @RAMP=%x RAM:\n", (unsigned int) Ramp);
	for(i=0;i<_MAX_MEMORY;i++) {
		printf("%02X", Ramp[i]);
		if((i&0xf)==15) printf("\n");
	}
}

int test_crypto_parameters( const uint8_t *argv[], int argc, char *name){
		fp_ctx_t ctx;
		cy_fp_t fp_a,fp_b, fp_r;
		cy_error_t error=CY_OK;
		/* The shared ram between program and library*/
	    uint8_t Ramp[_MAX_MEMORY];
		printf("\n ******************* %s *************************\n ",name );
	    printf("\n @RAMP=%x",  (unsigned int) Ramp);

	    memset(Ramp, 0x55, sizeof(Ramp));
		Print_RAMp(Ramp);


		CY_CHECK(cy_fp_init(&ctx, Ramp , _MAX_MEMORY, argc, argv));
		printf("\n test %s wrap with %s:", ctx.libname, name);
		Print_RAMp(Ramp);

		//error|=cy_fp_alloc(&ctx, 48, &fp_a);

		CY_CHECK(cy_fp_alloc(&ctx, 48, &fp_a));
		CY_CHECK(cy_fp_import(&ctx,secp384_r1_a,  48, &fp_a));

		CY_CHECK(cy_fp_alloc(&ctx, 48, &fp_b));
		CY_CHECK(cy_fp_import(&ctx,secp384_r1_b,  48, &fp_b));

		CY_CHECK(cy_fp_alloc(&ctx, 48, &fp_r));

		CY_CHECK(cy_fp_add(&ctx, &fp_a, &fp_b, &fp_r));


	//	printf("\n free");
		CY_CHECK(cy_fp_free(&fp_a));
		CY_CHECK(cy_fp_free(&fp_b));
		CY_CHECK(cy_fp_free(&fp_r));


		//printf("\n uninit");
		CY_CHECK(cy_fp_uninit(&ctx, Ramp , _MAX_MEMORY));

		end:
		//printf("\n end crypto");
		if(error==CY_OK) printf(" OK !\n");
		else printf(" KO\n");

		return error;

}



int main(){

	cy_error_t error=CY_OK;
	#ifndef _LIB256K1_BURRITOS
		CY_CHECK(test_crypto_parameters( argv_sec384, 2, "secp384r1"));
	#endif
	CY_CHECK(test_crypto_parameters( argv_sec256, 2, "sec256k1"));

	end:
		printf("\n Overall results:");
		if(error==CY_OK) printf(" OK !\n");
		else printf(" KO\n");
		return (int) error;
}
