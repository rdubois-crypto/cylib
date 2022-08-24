
/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_linksig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: this file implements the pedersen commitment as  described in   */
/* https://users.monash.edu.au/~kailiu/mypaper/computer-journal-2013.pdf	     */
/* Torben P. Pedersen:															 */
/* Non-Interactive and Information-Theoretic Secure Verifiable Secret Sharing.
 * CRYPTO 1991*/
/* */

/* note that G and H relative discrete log must be unknown. An example of implementation is
 * provided in cx_ec_swu.c, following Coron and Al hashing on Curve specification
*/
/**********************************************************************************/


#include "cy_errors.h"
#include "cy_def.h"
#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_ec.h"

cy_error_t cy_pedersen_commitment(const ec_ctx_t *ctx, const cy_ecpoint_t *G, const cy_ecpoint_t *H,
								  const cy_fp_t *x, const cy_fp_t *r,
								  cy_ecpoint_t *Commit){
  cy_error_t error;
  cy_ecpoint_t ec_temp;
  cy_ec_alloc(ctx, &ec_temp);
  /* xG */
  cy_ec_mult(G, x, Commit);
  /* rH*/
  cy_ec_mult(H, r, &ec_temp);
  /* Compute unknown elliptic relative discrete  log for pedersen and Plink: G, H, U*/
  cy_ec_add(Commit, &ec_temp, Commit);

  return error;
}

/* the vectorial version, with vec_a being a vector of value to commit to*/
cy_error_t cy_pedersen_vectorial_commitment(const ec_ctx_t *ctx, const cy_ecpoint_t *G, const cy_ecpoint_t *H,
		  const cy_fp *vec_x, size_t sizevec, const cy_fp_t *r,
		  cy_ecpoint_t *Commit)
{
   cy_error_t error;

   size_t i;

   for(i=0;i<sizevec;i++)
	   cy_pedersen_commitment(ctx, G, H,
	   								  &x[i], r,
	   								  Commit);

   return error;
}

