/**********************************************************************************/
/* Copyright (C) 2022 - - Renaud Dubois */
/* This file is part of cy_lib project						  */
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

#include "cy_linksig_Tryptich.h"


/***********************************************************************************************/
/******************* Prequel : Tryptich Internal functions   */
/***********************************************************************************************/

/* remember that N=n^m, thus n and m shall be relatively small values (<32) */
/* This function decompose the identity index l over the sigma matrix (identity decomposition for
 * commitment).
 */
static cy_error_t Tryptych_sigma_matrix(unsigned int l, unsigned int mat_sigma[m][n])
{
  cy_error_t error=CY_KO;
  return error;
}


/* compute the matrix which coefficients bj,i are the squares of input matrix aj,i's */
static cy_error_t Tryptych_squaredcoeff_matrix(const cy_tryptic_ctx_t *ctx, const cy_fp_t mat_fp_a[m][n], cy_fp_t mat_fp_a2[m][n] ){
	  cy_error_t error=CY_KO;
	  size_t
	  for(j=0;j<m;j++)
	  		for(i=0;i<n;i++)
	  			cy_fp_mul(&ctx->ec_ctx.ctx_fp_p, &mat_fp_a[j][i], &mat_fp_a2[j][i]);

	  return error;
}


/* compute the matrix which coefficients bj,i =  aj,i (1-2 sigmaj,i */
static cy_error_t Tryptych_mat_swap1m2sigma(const unsigned int mat_sigma[m][n], const cy_fp_t  mat_fp_in[m][n],		cy_fp_t mat_fp_out[m][n]){
	 cy_error_t error=CY_KO;
	  size_t j,i;

		  for(j=0;j<m;j++){
			  cy_fp_set_zero(&mat_fp_out[j][i]);
 	  		  for(i=0;i<n;i++){
		  			if(mat_sigma[j][i]==1) cy_fp_copy(&mat_fp_in[j][i], &mat_fp_out[j][i]);
		  			else cy_fp_neg(&mat_fp_in[j][i], &mat_fp_out[j][i]);
		  		}
		  }

	  return error;
}



/***********************************************************************************************/
/******************* Tryptich context handling */
/***********************************************************************************************/

cy_error_t Tryptych_Init(cy_tryptic_ctx_t *ctx, cy_gda_ctx_t *gda, uint8_t **argv){
  cy_error_t error;

  /* initialization of underlying elliptic field */

  /* check gda*/

  /* Compute unknown elliptic relative discrete  log for pedersen and Plink: G, H, U*/

  ctx->state=READY;
  return error;
}


/***********************************************************************************************/
/******************* Tryptich key management  */
/***********************************************************************************************/

cy_error_t Tryptych_KeyGen(const cy_tryptic_ctx_t *ctx, , const uint8_t *nonce, const int l, cy_tryptic_sk_t *privatekey)
{
	 cy_error_t error=CY_KO;

	 Tryptych_sigma_matrix(l, privatekey->mat_sigma);

	 return error;
}

/***********************************************************************************************/
/******************* Tryptich Proove  */
/***********************************************************************************************/

/* Tryptich is a 3 Pass protocol */
#define cy_Tryptych_Proove_All(...) return CY_NOT_IMPLEMENTED;


cy_error_t cy_Tryptych_Proove_Init(cy_tryptic_ctx_t *ctx, cy_tryptic_sk_t *privatekey, uint8_t *nonce, uint8_t *message,
		size_t m_t8, uint8_t *out){
	/* Declarations */
	cy_error_t error=CY_OK;
	ec_ctx_t *ec_ctx;
	size_t s=&ctx->ec_ctx.ctx_fp_p.t8_modular;
	cy_gda_ctx_t *gda=ctx->gda;
	size_t i,j;
	unsigned int mat_sigma[m][n]; /* the sigma matrix, translating identity l for commitment*/

	cy_fp_t J, rA, rB, rC, rD;

	cy_fp_t mat_fp_a[m][n];
	cy_fp_t mat_fp_ax1minus2s[m][n]; /* a(1-2s)*/

	/* Allocations */
	cy_fp_alloc(&ctx->ec_ctx.ctx_fp_p, s, &J);
	cy_fp_alloc(ctx->ec_ctx.ctx_fp_p, s, &rA);
	for(j=0;j<m;j++)
		for(i=0;i<n;i++)
			cy_fp_alloc(&ctx->ec_ctx.ctx_fp_p, s, &mat_fp_a[j][i]);
	/* Let R = {X0, . . . , XN−1} such that Xl = xlG.*/
	/* I. Compute J ≡ x−1U. */
	cy_fp_inv(&privatekey->fp_sk, &J);

	/* II. Run Plink(R, J; (l, xl)) → a (up to the verifier challenge).*/
	/* 1. Compute vectorial commitment A ≡ Com(a, rA)
		/* select random rA in F*/
	cy_fp_get_random(gda, &rA);
		/* select random aj,i i in [1-1], j in [0..m-1 of F*/
	for(j=0;j<m;j++){
				cy_fp_set_zero(mat_fp_a[j][0]);
				for(i=1;i<n;i++){
					cy_fp_get_random(gda, &mat_fp_a[j][i]);
					cy_fp_sub(&mat_fp_a[j][i], &mat_fp_a[j][0], &mat_fp_a[j][0]);/* Set sum{aj,0}m−1j=0 ≡ − sum aj,i forall i*/
				}
		}
		/* A ≡ Com(a, rA)*/
	cy_pedersen_vectorial_commitment(mat_fp_a, rA, ComA);

	/* 2. Compute vectorial commitment B ≡ Com(sigma, rB) (commit the sigma matrix indicating who signer is)*/

	/* 3. Compute vectorial commitment C ≡ Com( (1-2sigma)a, rC) */
	Tryptych_mat_swap1m2sigma(privatekey->mat_one_minus_2sigma, mat_fp_a, mat_fp_ax1minus2s);
	cy_fp_get_random(gda, &rC);
	cy_pedersen_vectorial_commitment(mat_fp_ax1minus2s, rC, ComC);

	/* 3. Compute vectorial commitment D ≡ Com(-a2, rD) */


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


/***********************************************************************************************/
/******************* Tryptich verify  */
/***********************************************************************************************/

/***********************************************************************************************/
/******************* Tryptich link  */
/***********************************************************************************************/


/* undef short constant name */
#undef n
#undef m

