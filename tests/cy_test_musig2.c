/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_cy_test_musig2.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing wrappers for elliptic arithmetic */
/**********************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cy_errors.h"


#include "cy_configuration.h"
#include "cy_errors.h"
#include "cy_fp.h"

#include "cy_ec.h"
#include "cy_hash.h"


#include "cy_musig2.h"

#include "cy_io_common_tools.h"
#include "test_vectors/test_vector_musig2.c"

/*
cy_error_t cy_musig_Verification_All(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s, cy_fp_t *c,
		const uint8_t *message, const size_t message_t8,
		boolean_t *flag_verif)
*/


/* Each signer generates a random secret key x ←$ Zp and returns the
corresponding public key X= g^x*/
cy_error_t cy_musig_KeyGen(cy_musig2_ctx_t *ctx,cy_ecpoint_t *X_pub)
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t x;
	cy_ecpoint_t G;
	cy_ec_alloc(ctx->ctx_ec, &G);
	cy_ec_get_generator(ctx->ctx_ec, &G);
	cy_ec_ctx_t  *ec_ctx= ctx->ctx_ec;

	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &x));

	/*II. Computations*/
	/*generates a random secret key x ←$ Zp*/
	CY_CHECK(cy_fp_get_random(ctx->gda, &x));
	/* returns the corresponding public key X=g^x*/
	CY_CHECK(cy_ec_scalarmult_fp(&G, &x, X_pub));

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&x));
	cy_ec_free(&G);

	end:
   	  return error;
}

int test_verif_core(cy_musig2_ctx_t *ctx)
{
  cy_ec_ctx_t *ec_ctx=ctx->ctx_ec;

  cy_error_t error=CY_KO;
  cy_ecpoint_t Generator, KeyAgg, R;
  cy_fp_t fp_s,fp_c;
  int flag=0;
  uint8_t buffer[64]={ 0x01, 0xEF, 0x15, 0xC1, 0x85, 0x99, 0x97, 0x1B, 0x7B, 0xEC, 0xED, 0x41, 0x5A, 0x40, 0xF0, 0xC7
		  , 0xDE, 0xAC, 0xFD, 0x9B, 0x0D, 0x18, 0x19, 0xE0, 0x3D, 0x72, 0x3D, 0x8B, 0xC9, 0x43, 0xCF, 0xCA
		  , 0x00, 0x56, 0x68, 0x06, 0x0A, 0xA4, 0x97, 0x30, 0xB7, 0xBE, 0x48, 0x01, 0xDF, 0x46, 0xEC, 0x62
		  , 0xDE, 0x53, 0xEC, 0xD1, 0x1A, 0xBE, 0x43, 0xA3, 0x28, 0x73, 0x00, 0x0C, 0x36, 0xE8, 0xDC, 0x1F
		    };

  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n Verif Musig Core:");

  CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
  CY_CHECK(cy_ec_alloc(ec_ctx, &Generator));
  CY_CHECK(cy_ec_alloc(ec_ctx, &R));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_s));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_c));

  CY_CHECK( cy_fp_import(s, sizeof(s), &fp_s));
  CY_CHECK(cy_fp_import(c, sizeof(c), &fp_c));
  CY_CHECK( cy_ec_import2(R_x, sizeof(R_x), R_y, sizeof(R_y), &R));
  CY_CHECK( cy_ec_import2(Key_Agg_X, sizeof(Key_Agg_X), Key_Agg_Y, sizeof(Key_Agg_Y),&KeyAgg ));

  CY_CHECK(cy_musig_Verification_Core(ctx, &KeyAgg,&R, &fp_s, &fp_c,& flag));
  if(flag!=CY_TRUE){
	  error=CY_KO;
	  goto end;
  }


  CY_CHECK(cy_ec_free( &KeyAgg));
  CY_CHECK(cy_ec_free( &R));
  CY_CHECK(cy_fp_free( &fp_s));
  CY_CHECK(cy_fp_free( &fp_c));

  printf(" OK");
  end:
  	  return error;
}

int test_musig_unit()
{
	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;
	uint8_t Ramp[ _EC_ZONE_T8 ];
	size_t nb_users=4;
	cy_musig2_ctx_t musig_ctx;

	/* Initiate elliptic structure*/
	CY_CHECK(cy_ec_init(&ec_ctx, Ramp,_EC_ZONE_T8, CY_CURVE_Stark256, NULL));
	musig_ctx.ctx_ec=&ec_ctx;

	/* Initiate gda*/
	musig_ctx.gda=&bolos_gda_component;

	printf("\n\n /************************ Test Musig2 Protocol:");
	error=test_verif_core(&musig_ctx);


	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO");
			return error;
}




