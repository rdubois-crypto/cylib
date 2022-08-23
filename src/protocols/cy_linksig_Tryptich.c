/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_linksig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: this file implements the linkable signatures  described in   */
/* https://users.monash.edu.au/~kailiu/mypaper/computer-journal-2013.pdf	     */
/* "Triptych: logarithmic-sized linkable ring signatures with */
/*applications*/
/*Sarang Noether and Brandon Goodell             */
/* Note that the 3-Passes of the Signature are split into init and update */
/* To shorten input sizes, the nonce given to the signature is used as input of the
 * library drbg, same for the verification challenge
 */
/**********************************************************************************/

#include "cy_errors.h"
#include "cy_def.h"
#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_linkable_sign.h"
#include "cy_gda_component.h"

/* Constante n and m of protocol set to 2 and 10 for 2^10 Ring size */
#define n 2
#define m 10

typedef enum
{
    READY,
    PLINK_WAIT,
    DATA_WAIT,
	UPDATE_END,

} tryptych_state;

struct cy_tryptich_ctx_s {
	ec_ctx_t ec_ctx;
	tryptych_state state;
	cy_gda_ctx_t *gda;

    void *additional;
    /* three generators with proven unknown relative discrete log*/
    cy_ec_point_t G;
    cy_ec_point_t H;
    cy_ec_point_t U;

};


typedef struct cy_tryptich_ctx_s cy_tryptic_ctx_t;


struct cy_tryptich_key_s {
	cy_fp_t fp_sk;
};

typedef struct cy_tryptich_key_s cy_tryptic_sk_t;



cy_error_t Tryptych_Init(cy_tryptic_ctx_t *ctx, cy_gda_ctx_t *gda, uint8_t **argv){
  cy_error_t error;

  /* initialization of underlying elliptic field */

  /* check gda*/

  /* Compute unknown elliptic relative discrete  log for pedersen and Plink: G, H, U*/

  ctx->state=READY;
  return error;
}



cy_error_t Tryptych_Proove_Init(cy_tryptic_ctx_t *ctx, cy_tryptic_sk_t *privatekey, uint8_t *nonce, uint8_t *message, size_t m_t8, uint8_t *out){
	/* Declarations */
	cy_error_t error=CY_OK;
	ec_ctx_t *ec_ctx;
	size_t s=&ctx->ec_ctx.ctx_fp_p.t8_modular;
	cy_gda_ctx_t *gda=ctx->gda;
	size_t i,j;

	cy_fp_t J;
	cy_fp_t rA;
	cy_fp_t a[m][n];
	/* Allocations */
	cy_fp_alloc(&ctx->ec_ctx.ctx_fp_p, s, &J);
	cy_fp_alloc(ctx->ec_ctx.ctx_fp_p, s, &rA);
	for(j=0;j<m;j++)
		for(i=0;i<n;i++)
			cy_fp_alloc(&ctx->ec_ctx.ctx_fp_p, s, &a[j][i]);
	/* Let R = {X0, . . . , XN−1} such that Xl = xlG.*/
	/* I. Compute J ≡ x−1U. */
	cy_fp_inv(&privatekey->fp_sk, &J);

	/* II. Run Plink(R, J; (l, xl)) → a (up to the verifier challenge).*/
	/* 1. Compute vectorial commitment A ≡ Com(a, rA)
		/* select random rA in F*/
	cy_fp_get_random(gda, &rA);
		/* select random aj,i i in [1-1], j in [0..m-1 of F*/
	for(j=0;j<m;j++){
				cy_fp_set_zero(a[j][0]);
				for(i=1;i<n;i++){
					cy_fp_get_random(gda, &a[j][i]);
					cy_fp_sub(&a[j][i], &a[j][0], &a[j][0]);/* Set sum{aj,0}m−1j=0 ≡ − sum aj,i forall i*/
				}
		}
		/* A ≡ Com(a, rA)*/
	cy_pedersen_vectorial_commitment(a, rA);
	/* 2. Compute vectorial commitment B ≡ Com(sigma, rB) (commit to who signer is)*/


	ctx->state=  PLINK_WAIT;

	return error;
}


cy_error_t Tryptych_Proove_First_Update(cy_tryptic_ctx_t *ctx, cy_Tryptic_sk_t *privatekey, uint8_t *verifierchallenge, size_t m_t8, uint8_t *out){
	cy_error_t error=CY_OK;

	/*Set ξ ≡ H(M, R, a).*/
	/*Run Plink(ξ) → z (after the verifier challenge).*/
	/*Return σ = (a, z, J) */


	return error;
}

/* undef short constant name */
#undef n
#undef m

