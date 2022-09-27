/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature Musig2 signatures source code */
/* https://eprint.iacr.org/2020/1261.pdf             */
/* note that some constant aggregating values could be precomputed	*/
/**********************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_musig2.h"

#ifdef _TESTED
/* Initialize Musig with Signatures functions, number of users, hash functions */
cy_error_t cy_musig_SetUp(cy_musig2_ctx_t *ctx,  uint8_t *Ramp, size_t sizeRamp,
		  uint8_t *initializer, size_t init_t,
        const cy_hash_ctx_t *H){

	cy_error_t error=CY_KO;



	end:
			return error;
}
/*************************************************************/
/* Computation of ai=H(L, X_i)*/
/*************************************************************/
static cy_error_t cy_musig_KeyAggCoeff(cy_musig2_ctx_t *ctx, const cy_ecpoint_t *publickeys, size_t n_users, size_t index,
		cy_fp_t *ai)
{
	uint8_t buffer[MAX_MUSIG_EC_T8];
	uint8_t hash[MAX_MUSIG_HASH_T8];

	cy_error_t error=CY_KO;

	/*H_agg is chosen to be H(X1||X2||\ldots||X_i), where || denotes concatenation */
	for(i=0;i<n_users;i++){
		CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, publickeys+i,ctx->ctx_ec->t8_modular_p,buffer));
		ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	}
	CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, publickeys+index,ctx->ctx_ec->t8_modular_p,buffer));
	ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	ctx->H->Hash_Final((void *)ctx->H, hash);

	/*import hash of hash function size to Fq scalar field, assumption is that hash output has same bitsize as field*/
	/* todo: make modular reduction of buffer */
	CY_CHECK(cy_fp_import(&ctx->ctx_ec->ctx_fp_q, hash,ctx->H->Hash_t8,ai));

	end:
		return error;
}

/*************************************************************/
/* Single user functions*/
/*************************************************************/

/* Each signer generates a random secret key x ←$ Zp and returns the
corresponding public key X= g^x*/
cy_error_t cy_musig_KeyGen(cy_musig2_ctx_t *ctx, const size_t n_users, cy_ecpoint_t *X_pub)
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t x;
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */
	CY_CHECK(cy_fp_alloc(&x, ctx->ctx_ec->ctx_fp_p->t8_modular));

	/*II. Computations*/
	/*generates a random secret key x ←$ Zp*/
	CY_CHECK(cy_fp_get_random(ctx->gda, &x));
	/* returns the corresponding public key X=g^x*/
	CY_CHECK(cy_ec_scalarmult_fp(G, &x, X_pub));

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&x));

	end:
   	  return error;
}



cy_error_t cy_musig_Sign_Round1_all(const cy_musig2_ctx_t *ctx,const size_t n_users,  const size_t index, cy_ecpoint_t *Rijs_Round1)
{
	cy_error_t error=CY_KO;
	size_t i;
	cy_fp_t x;
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */

	CY_CHECK(cy_fp_alloc(&x, ctx->ctx_ec->ctx_fp_p->t8_modular));
	/* For each j ∈ {1, . . . , ν}*/
	for(i=0;i<n_users; i++)
	{
		/*generates a random nonce key ri,j ←$ Zp*/
		CY_CHECK(cy_fp_get_random(ctx->gda, &x));
		/*computes R1,j =g^r1,j*/
		CY_CHECK(cy_ec_scalarmult_fp(G, &x, Rijs_Round1+i));
	}

	CY_CHECK(cy_fp_free(&x));

	end:
   	  return error;
}

/* Sign' function of paper*/
/* ai is computed externally using cy_musig_KeyAggCoeff*/
/* s_i is the final round two ouput for user i*/
/* vec_sigagg is the aggregated output signature of round1 */
/* todo later: to reduce memory consumption, gda state for nonces may be saved*/
cy_error_t cy_musig_Sign_Round2_all(const cy_musig2_ctx_t *ctx,const size_t n_users,
		const cy_fp_t *ai, const cy_fp_t *privatekey_xi,
		const cy_ecpoint_t **vec_sigagg, const cy_fp_t **vec_nonces,
		const uint8_t *message, const size_t message_t8,
		cy_ecpoint_t R, cy_fp_t *s_i)
{
	cy_error_t error=CY_KO;

	cy_ecpoint_t temp;

	cy_ecpoint_t *Ri;/*pointer to the i-th part of aggregate sig*/
	cy_fp_t b;/*Hnon(X, (R1. . . , Rν	), m)*/
	cy_fp_t acc_b;/* local variable to accumulate the b^j's*/
	cy_fp_t acc_rbj;/* local variable to accumulate the rij.b^j's*/


	uint8_t buffer[MAX_MUSIG_EC_T8];
	uint8_t c[MAX_MUSIG_HASH_T8];
	size_t i,j;

	/* Allocations */
	CY_CHECK(cy_fp_alloc(&ctx->ctx_ec->ctx_fp_q, &b));
	CY_CHECK(cy_fp_alloc(&ctx->ctx_ec->ctx_fp_q, &acc_b));
	CY_CHECK(cy_fp_alloc(&ctx->ctx_ec->ctx_fp_q, &acc_rbj));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &temp));


	/* I. Compute Hnon(X, (R1. . . , Rν	), m)*/
	/*H_non is chosen to be H(X1||X2||\ldots||X_n ||(R1. . . , Rν	)||m), where || denotes concatenation */
	/* append Xi's*/
	for(i=0;i<n_users;i++){
			CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, publickeys+i,ctx->ctx_ec->t8_modular_p,buffer));
			ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	}
	/* append Rij's*/
	for(i=0;i<n_users;i++){
		*Ri=vec_sigagg[i];
		for(j=0;j<n_users;j++){
				CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, Ri+j,ctx->ctx_ec->t8_modular_p,buffer));
				ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
			}
		}

	/* append m*/
	ctx->H->Hash_Update((void *)ctx->H, message, message_t8);
	ctx->H->Hash_Final((void *)ctx->H, hash);
	CY_CHECK(cy_fp_import(&ctx->ctx_ec->ctx_fp_q, hash,ctx->H->Hash_t8,&b));

	/*Accumulate R_j^(b^j-1) in R*/
	/*c= Hsig(X, R, m)*/
	/*s1= ca1x1 +Xνj=1r1,j b j−1 mod p*/
	CY_CHECK(cy_ec_copy(ctx->ctx_ec, vec_sigagg[0], &R)); /*R=R1^b*/
	CY_CHECK(cy_fp_copy(&ctx->ctx_ec->ctx_fp_q, &b, &acc_b)); /*b*/
	CY_CHECK(cy_fp_copy(&ctx->ctx_ec->ctx_fp_q, vec_nonces[0], s_i)); /* accumulation in s_i initiated to r1*b^0=r1*/

	for(j=1;j<n_users;i++){
		CY_CHECK(cy_ec_scalarmult_fp(vec_sigagg[j], &acc_b, &temp)); 	/*Rj^b^j-1*/
		CY_CHECK(cy_ec_add( &temp, &R, &R)); 	/*R=Rj^b^j-1*/
		CY_CHECK(cy_fp_mult(&acc_b, &b,  &acc_b)); 	/*b^j for next loop step*/
		/* accumulation in s_i initiated to r1*b^0=r1*/
		CY_CHECK(cy_fp_copy(&ctx->ctx_ec->ctx_fp_q, vec_nonces[j],&acc_rbj)); /* rij*/
		CY_CHECK(cy_fp_mul(&ctx->ctx_ec->ctx_fp_q, &acc_b,&acc_rbj, acc_rbj)); /* rij.b^j */
		CY_CHECK(cy_fp_add(&acc_b, s_i,  &acc_rbj, s_i)); 	/*s_i+=rij.b^j */

		/* accumulate X in Hsig*/
		CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, Ri+j,ctx->ctx_ec->t8_modular_p,buffer));
		ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	}
	/* append R in Hashin*/
	CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, R,ctx->ctx_ec->t8_modular_p,buffer));
	ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	/* append message in Hashin*/
	ctx->H->Hash_Update((void *)ctx->H, message, message_t8);
	ctx->H->Hash_Final((void *)ctx->H, c); /* final c value */

	/*s1= ca1x1 + sum(r1jb^j)*/
	CY_CHECK(cy_fp_mul(&ctx->ctx_ec->ctx_fp_q, ai, privatekey_xi, &acc_rbj));/*a1.x1*/
	CY_CHECK(cy_fp_mul(&ctx->ctx_ec->ctx_fp_q, &acc_rbj, &c, &acc_rbj));/*c.a1.x1*/
	CY_CHECK(cy_fp_add(&acc_b, s_i,  &acc_rbj, s_i)); 	/*s_i+=rij.b^j */


	/* free pointers*/
	CY_CHECK(cy_fp_free(&acc_b));
	CY_CHECK(cy_fp_free(&acc_rbj));
	CY_CHECK(cy_fp_free(&b));

	CY_CHECK(cy_ec_free(&temp));

	end:
	   	  return error;
}
/*************************************************************/
/* Aggregator functions*/
/*************************************************************/
/*  aggregate key corresponding to L is X=Prod_1^n X^ai */
cy_error_t cy_musig_KeyAgg(cy_musig2_ctx_t *ctx, const size_t n_users, const cy_ecpoint_t *publickeys, cy_ecpoint_t *keyagg){
	size_t i;
	cy_ecpoint_t temp;
	cy_fp_t ai;/*H(L,Xi)*/

	cy_fp_alloc(&ctx->ctx_ec->ctx_fp_p, &ai)
	cy_ec_alloc(ctx->ctx_ec, &temp);
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */


	cy_error_t error=CY_KO;

	/* set accumulator to infinity point*/
	CY_CHECK(cy_ec_setInfty(ctx->ctx_ec, &temp));
	/* compute sum of X^H(L,Xi)*/
	for(i=0;i<n_users;i++)
	{

		cy_musig_HashToScalarField(ctx, publickeys, n_users, i, &ai);
		CY_CHECK(cy_ec_scalarmult_fp(G,  &ai, temp));
		CY_CHECK(cy_ec_add(&temp, publickeys+i, &temp));
	}

	CY_CHECK(cy_ec_copy(ctx->ctx_ec, &temp, &keyagg));
	CY_CHECK(cy_ec_free(&temp));
	CY_CHECK(cy_fp_free(&ai));

	end:
		return error;
}

cy_error_t cy_musig_SigAgg_Round1(cy_musig2_ctx_t *ctx, const size_t n_users, const cy_ecpoint_t **vec_sig, cy_ecpoint_t **vec_sigagg){
	size_t i,j ;
	cy_ecpoint_t keyagg;
	cy_error_t error=CY_KO;
	cy_ecpoint_t *sig_i;/* the sig of i-th user*/
	cy_ecpoint_t *sigaggj_j; /* the j-th coordinates of the aggregation*/
	/* copy first pubkey in res*/

	/* compute vectorial sum of signatures*/
	for(j=0;j<n_users;j++)
	{
		CY_CHECK(cy_ec_copy(ctx->ctx_ec, &vec_sig[0]+j, &vec_sigagg[0]));/*copy first user sig to begin sum*/
		for(i=1;i<n_users;i++)
		{
			sig_i=*vec_sig+i;/* the i-th user sig*/
			CY_CHECK(cy_ec_add(vec_sigagg+i, sig_i+j, vec_sigagg));
		}
	}
	end:
		return error;
}

/*SignAgg', output the sum s=sum s_i mod p*/
cy_error_t cy_musig_SigAgg_Round2(const size_t n_users, const cy_fp_t **vec_sig2, cy_fp_t *S)
{
	size_t j ;
	cy_error_t error=CY_KO;

	CY_CHECK(cy_fp_copy(S->ctx, &vec_sig2[0], S));/*copy first user sig to begin sum*/

	for(j=0;j<n_users;j++)
		{
			CY_CHECK(cy_fp_add(S->ctx, &vec_sig2[j], S, S));/*accumulate signature in sum*/
		}

	end:
			return error;
}

/*************************************************************/
/* Verification function									 */
/*************************************************************/
/* the two part of signatures (R,S) received from previous round, */
/* R and c being computable from aggregation of Rijs_Round1, S being output of Round2 sign'*/

cy_error_t cy_musig_Verification_All(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s,
		const uint8_t *message, const size_t message_t8,
		boolean_t *flag_verif){

	size_t j ;
	cy_error_t error=CY_KO;
	*flag_verif=CY_FALSE;
	cy_ecpoint_t ec_temp1, ec_temp2;

	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp1));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp2));

	/* Accept iff g^s = R.X^c, beware of multiplicative notations*/
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */
	CY_CHECK(cy_ec_scalarmult_fp(ctx->ctx_ec, G, s, &ec_temp1)); 	/*g^s*/
	/* todo: c should be recomputed here internally */
	/* append R in Hashin*/

	//CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, R,ctx->ctx_ec->t8_modular_p,buffer));
	//ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	/* append message in Hashin*/
	//ctx->H->Hash_Update((void *)ctx->H, message, message_t8);
	//ctx->H->Hash_Final((void *)ctx->H, c); /* final c value */


	CY_CHECK(cy_musig_Verification_Core(ctx, Key_agg, R, fp_s, fp_c, flag_verif));

	end:
		return error;
}






#endif
/*************************************************************/
/* Verification function									 */
/*************************************************************/
/* the two part of signatures (R,S) received from previous round, */
/* R and c being computable from aggregation of Rijs_Round1, S being output of Round2 sign'*/

cy_error_t cy_musig_Verification_Core(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, cy_fp_t *fp_c,
		boolean_t *flag_verif){

	cy_error_t error=CY_KO;
	*flag_verif=CY_FALSE;
	cy_ecpoint_t ec_temp1, ec_temp2, G;

	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp1));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp2));

	/* Accept iff g^s = R.X^c, beware of multiplicative notations*/
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &G));
	CY_CHECK(cy_ec_get_generator(ctx->ctx_ec, &G));

	CY_CHECK(cy_ec_scalarmult_fp(fp_s, &G, &ec_temp1)); 	/*g^s*/
	/* todo: c should be recomputed here internally */
	/* append R in Hashin*/

	//CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, R,ctx->ctx_ec->t8_modular_p,buffer));
	//ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	/* append message in Hashin*/
	//ctx->H->Hash_Update((void *)ctx->H, message, message_t8);
	//ctx->H->Hash_Final((void *)ctx->H, c); /* final c value */


	CY_CHECK(cy_ec_scalarmult_fp(fp_c, Key_agg,  &ec_temp2)); 	/*X^c*/
	CY_CHECK(cy_ec_add( &ec_temp2, R, &ec_temp2)); 	/*R.X^c*/

	/* final verification */
	CY_CHECK(cy_ec_iseq( &ec_temp1, &ec_temp2, 	flag_verif));

	CY_CHECK(cy_ec_free( &ec_temp1));
	CY_CHECK(cy_ec_free( &ec_temp2));
	CY_CHECK(cy_ec_free( &G));

	end:
		return error;
}
