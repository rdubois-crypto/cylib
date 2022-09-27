/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_HashPedersen.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Modified  Stark Hash Pedersen*/
/* note: this is a straight translation from the implementation available here:
 * https://github.com/LedgerHQ/app-ethereum/blob/develop/src/stark_utils.c
 * 
 */
/**********************************************************************************/
#include <stddef.h>
#include <stdint.h>

#include "cy_configuration.h"
#include "cy_bn.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_HashPedersen.h"


static cy_error_t ec_muladd(cy_pedersen_ctx_t *ctx, cy_ecpoint_t *acc, cy_fp_t *a, int index)
{
   cy_error_t error;
   cy_ecpoint_t ec_temp;

   CY_CHECK(cy_ec_alloc(ctx->ec_ctx, &ec_temp));
   CY_CHECK(cy_ec_scalarmult_fp(a, &ctx->P[index], &ec_temp));
   CY_CHECK(cy_ec_add(acc, &ec_temp, acc));

   end:
   return error;           
}

/* init PShift, P0 to P3*/
cy_error_t pedersen_init(cy_ec_ctx_t *ec_ctx, cy_pedersen_ctx_t *ctx)
{
   cy_error_t error;
   size_t i;

   ctx->ec_ctx=ec_ctx;

   for(i=0;i<_NUM_PEDPOINT;i++){
	   CY_CHECK(cy_ec_alloc(ec_ctx, &ctx->P[i]));
   }
   CY_CHECK(cy_ec_alloc(ec_ctx, &ctx->ShiftPoint));

   CY_CHECK(cy_ec_import(Pedersen_P0, Stark_SIZE_u8, &ctx->P[0] ));
   CY_CHECK(cy_ec_import(Pedersen_P1, Stark_SIZE_u8, &ctx->P[1] ));
   CY_CHECK(cy_ec_import(Pedersen_P2, Stark_SIZE_u8, &ctx->P[2] ));
   CY_CHECK(cy_ec_import(Pedersen_P3, Stark_SIZE_u8, &ctx->P[3] ));

   CY_CHECK(cy_ec_import(Pedersen_Shift, Stark_SIZE_u8, &ctx->ShiftPoint ));

   CY_CHECK(cy_fp_import(mask248_low, Stark_SIZE_u8, &ctx->mask248_low) );



   end:
   return error;           
}

static cy_error_t pedersen(cy_pedersen_ctx_t *ctx, cy_fp_t *res, cy_fp_t *a, cy_fp_t *b){
   cy_error_t error;
   cy_ecpoint_t ec_Hash;
   cy_fp_t fp_temp;

   CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, Stark_SIZE_u8, &fp_temp));


   CY_CHECK(cy_ec_alloc(ctx->ec_ctx, &ec_Hash));
   CY_CHECK(cy_ec_import(Pedersen_Shift, Stark_SIZE_u8, &ec_Hash )); /* R=ShiftPoint*/


   CY_CHECK(cy_bn_or( a->bn, ctx->mask248_low.bn, fp_temp.bn) );		 /*R+= low_a*P0*/
   CY_CHECK(ec_muladd(ctx, &ec_Hash, &fp_temp, 0));

   CY_CHECK(cy_wrap_bolos_bn_shift_r(248, a->bn));							/*R+= low_a*P1*/
   CY_CHECK( ec_muladd(ctx, &ec_Hash, a, 1));

   CY_CHECK(cy_bn_or( b->bn, ctx->mask248_low.bn, fp_temp.bn) );		 /*R+= low_b*P2*/
   CY_CHECK(ec_muladd(ctx, &ec_Hash, &fp_temp, 2));

   CY_CHECK(cy_wrap_bolos_bn_shift_r(248, b->bn));							/*R+= low_a*P1*/
   CY_CHECK(ec_muladd(ctx, &ec_Hash, b, 3));

   /* getX*/
   CY_CHECK( cy_ec_getX(&ec_Hash, res));

   CY_CHECK( cy_ec_free(&ec_Hash));
   end:
   return error;           
   }
 
/* note: for a init/update/final version, pedersen could be considered as a 252 bits block sized hash function*/
/* this implementation is intended to be compatible with Cairo */   
cy_error_t pedersen_hash(cy_pedersen_ctx_t *ctx, cy_fp_t *data, size_t data_fp_length)
{
  cy_error_t error;
  size_t i;
  cy_fp_t fp_hash;
  cy_fp_t fp_temp;


  CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, ctx->ec_ctx->ctx_fp_p->t8_modular, &fp_hash));
  CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, ctx->ec_ctx->ctx_fp_p->t8_modular, &fp_temp));

  CY_CHECK(cy_fp_set_zero(&fp_hash));

  /*hash=pedersen(0, data[0]); 			#h(0,x1)*/
  for(i=1;i<data_fp_length;i++){
	  CY_CHECK(pedersen(ctx, &fp_hash, &fp_hash, &(data[i])) );
  }
  CY_CHECK(cy_fp_from_u32( (uint32_t) data_fp_length, &fp_temp));

  CY_CHECK(pedersen(ctx, &fp_hash, &fp_hash, &fp_temp));

  CY_CHECK(cy_fp_free(&fp_hash));
   end:
   return error;           
}   
   
cy_error_t pedersen_uninit(cy_pedersen_ctx_t *ctx){
  cy_error_t error;
  size_t i;

  for(i=0;i<_NUM_PEDPOINT;i++){
 	   CY_CHECK(cy_ec_free(&(ctx->P[i])));
    }

  CY_CHECK(cy_ec_free(&ctx->ShiftPoint));
  end:
     return error;
}
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
             


