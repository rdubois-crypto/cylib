/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos.h								   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib prime fied arithmetic                   */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"



#define word32_from_be(a) (a[0]+(a[1]<<8)+(a[2]<<16)+(a[3]<<24) )

#define _2POWB {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} /*2^128*/
#define _2POWBm1 {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff ,0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff} 	     /* 2^128-1*/

// When the BN_CTX structure is initialized (not NULL), it also means
// 'bn_is_locked'.


/*****************************************************************************/
/*	I. completing missing Speculos functions								 */
/*****************************************************************************/

/* computation of -x mod n=n-x*/
cx_err_t cy_mod_neg(cx_bn_t o_r, cx_bn_t i_x, cx_bn_t i_n)
{
	 cx_err_t error;

	 CX_CHECK(sys_cx_bn_mod_sub(o_r, i_n, i_x, i_n));/*  n-x mod n*/

	 end:
		 return error;
}

/* a fork of cx version with possible even value of n*/
cx_err_t cy_mpi_mod_pow(cx_mpi_t *r, const cx_mpi_t *a, const cx_mpi_t *e,
                        const cx_mpi_t *n)
{
  // This Function will compute r = pow(a, e) % n
  cx_err_t error;

  BN_CTX *local_bn_ctx =cx_get_bn_ctx();

  if ( cx_mpi_is_zero(e)) {
    error = CX_INVALID_PARAMETER;
  } else if (BN_mod_exp_recp(r, a, e, n, local_bn_ctx) == 0) {
    error = CX_INTERNAL_ERROR;
  } else {
    error = CX_OK;
  }
  return error;
}

cx_err_t sys_cy_bn_mod_pow(cx_bn_t bn_r, const cx_bn_t bn_a,
                           const uint8_t *bytes_e, uint32_t len_e,
                           const cx_bn_t bn_n)
{
  cx_err_t error;
  cx_mpi_t *r, *a, *e, *n;
  cx_bn_t bn_e;

  CX_CHECK(sys_cx_bn_alloc_init(&bn_e, cx_get_bn_size(), bytes_e, len_e));
  CX_CHECK(cx_bn_rabm_to_mpi(bn_r, &r, bn_a, &a, bn_e, &e, bn_n, &n));


  CX_CHECK(cy_mpi_mod_pow(r, a, e, n));
end:
  return error;
}


/* an emulation of montgomery conversion, not present in speculos, compute a*R mod p*/
/* conversion to montgomery is equivalent to modular multiplication by 2^128*/
cy_error_t cy_mont_to_montgomery(cx_bn_t x, const cx_bn_t z, const cy_bn_mont_ctx_t *ctx)
{
	 cx_err_t error=CY_KO;
	 cx_bn_t *p=(cx_bn_t *) ctx->n;

	 CY_CHECK(sys_cx_bn_mod_mul( x, ctx->h,  z, *p));

	 end:
	   return error;
}

/* an emulation of cx_mont_mul, not present in speculos, compute a*b*R mod p*/
cy_error_t cy_mont_mul(cx_bn_t r, const cx_bn_t a, const cx_bn_t b, const cy_bn_mont_ctx_t *ctx){
	cy_error_t error=CY_KO;
	size_t field_t8;

	cx_bn_t  temp;

	CY_CHECK(sys_cx_bn_nbytes(ctx->h, &field_t8));

	CY_CHECK(sys_cx_bn_alloc(&temp,  field_t8));

	cx_bn_t *p=(cx_bn_t *) ctx->n;
	CY_CHECK(sys_cx_bn_mod_mul( temp, a, b,  *p));
	CY_CHECK(sys_cx_bn_mod_mul( r, temp, ctx->h,  *p));

	 /* destroy*/
	 sys_cx_bn_destroy(&temp);

	end:
	return error;
}

/* conversion from montgomery is equivalent to montgomery multiplication by 1*/
cy_error_t cy_mont_from_montgomery (cx_bn_t z, const cx_bn_t x, const cx_bn_mont_ctx_t *ctx)
{
	 cx_err_t error=CY_KO;
	 cx_bn_t  temp;
	 size_t field_t8;

	 CY_CHECK(sys_cx_bn_nbytes (ctx->h, &field_t8));
	 CY_CHECK(sys_cx_bn_alloc(&temp,  field_t8));

	 CY_CHECK(sys_cx_bn_set_u32(temp, 1));

	 CY_CHECK(cy_mont_mul( z, temp, x,  ctx));

	 CY_CHECK(sys_cx_bn_destroy(&temp));

	 /* destroy*/
	 CY_CHECK(sys_cx_bn_destroy(&temp));

		 end:
		   return error;
}

cy_error_t cy_mont_alloc (cx_bn_mont_ctx_t *ctx, size_t t8_length){
	 cx_err_t error;

	 CY_CHECK(sys_cx_bn_alloc(&ctx->n,  t8_length));
	 CY_CHECK(sys_cx_bn_alloc(&ctx->h,  t8_length));
	 CY_CHECK(sys_cx_bn_alloc(&ctx->mpinv,  t8_length));

	 printf("\n mont alloc:%x %x %x with size %d",ctx->n, ctx->h, ctx->mpinv, (int) t8_length);

	 /* V. Return*/
	 end:
	 return error;
}

/* computation of Montgomery constants, it is a temporary chicken or egg implementation as bn_mod_pow should use montgomery rep.*/
cy_error_t cy_mont_init(	cy_bn_mont_ctx_t * 	ctx, const cy_bn_t 	n ){
	/* I.Declarations*/

	 cx_err_t error;
	 size_t sizen;
	 uint8_t tu8_basis[]=_2POWB;
	 uint8_t tu8_basism1[]=_2POWBm1;
	 cx_bn_t basis, temp;

	 CY_CHECK(sys_cx_bn_nbytes(n, &sizen));
	/* II. Allocations*/
	 CY_CHECK(sys_cx_bn_alloc_init(&basis, sizen, tu8_basis, (size_t) sizeof(tu8_basis)));
	 CY_CHECK(sys_cx_bn_alloc(&temp,  sizen));


	 /* III. Computations*/
	 /* copy modulus*/
	 CY_CHECK(sys_cx_bn_copy( (ctx->n),n));
	 /* -p^-1 mod 2^sizeword*/
	 CY_CHECK(sys_cx_bn_reduce(temp, n,  basis));

//	 printf("\n mont alloc pour pow:%x %x %x %x  %x %x",ctx->n, ctx->h, ctx->mpinv, temp, basis,n);
//	 printf("sizeof n bm bm1 = %d %d %d", (int) sizen, (int) sizeof(tu8_basis), (int) sizeof(tu8_basism1));

	 CY_CHECK(sys_cy_bn_mod_pow( ctx->h,temp, tu8_basism1, sizeof(tu8_basism1), basis));/*1/P mod 2^n*/

	 CY_CHECK(cy_mod_neg(ctx->mpinv, ctx->h, basis));			  /*-1/P*/

	/* 2^bitsizeof(n) mod n */
	 CX_CHECK(sys_cx_bn_xor(basis, basis, basis));/* zero*/
	 CX_CHECK(sys_cx_bn_set_bit(basis, (sizen<<3)-1));/*2^(sizeofp)-1 to fit in memory before reduction*/
	 CX_CHECK(sys_cx_bn_mod_add(ctx->h, basis, basis, n));/* 2^(bitsize(p))*/

	 /* IV. Free*/
	 sys_cx_bn_destroy(&temp);
	 sys_cx_bn_destroy(&basis);

	 /* V. Return*/
	 end:
	 return error;
}


cy_error_t cy_mont_init2(cy_bn_mont_ctx_t * 	ctx, const cx_bn_t 	n,const cx_bn_t 	h )
{
	/* I.Declarations*/
    cx_err_t error;
    /* III. Computations*/
	CY_CHECK(sys_cx_bn_copy( (ctx->n),n));
	CY_CHECK(sys_cx_bn_copy( (ctx->h),h));
 //   CY_CHECK(cy_mod_neg(ctx->mpinv, ctx->h, basis));			  /*-1/P*/

	 /* V. Return*/
	 end:
	 return error;
}


/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling			 */
/*****************************************************************************/

/* The syntax of a bolos initializer is
 *
 * argv[0]:[0][1][2]...[WordByteSize-1]: bytesize of modulus
 * argv[1]:[WordByteSize...] : MSB asn1 encoding of modulus
 *
 * memory mapping of the library
 * 1: n index in SE, modulus, MSB encoding
 * 2: h index in SE, first montgomery constant
 * 3: mpinv index in SE,, second montgomery constant= -p^-1 mod 2^(bitsizeof(word_t))
 */

cy_error_t wrap_bolos_fp_init(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]){

	cy_error_t error=CY_KO;

	cx_bn_t bn_p;

	if(argc!=2) {
		return CY_KO;
	}
	/*control memory limit*/
	if(t8_Memory>_MAX_MEMORY){
		error=CY_KO;
		goto end;
	}

	ps_ctx->Shared_Memory=pu8_Mem; /*set Memory*/

    ps_ctx->montgomery_ctx=(void*) (ps_ctx->Shared_Memory);
    ps_ctx->modular=(void*) (ps_ctx->Shared_Memory);

	strcpy(ps_ctx->libname, BOLOS_LIBNAME);
	ps_ctx->is_initialized=CY_LIB_INITIALIZED;

	ps_ctx->t8_modular=word32_from_be(argv[0]);
	//printf("\n modular size=%d", (int) ps_ctx->t8_modular);

	/*
	for(i=0;i<ps_ctx->t8_modular;i++) pu8_Mem[i+offset]=argv[1][i];
	offset+=i;*/

	/* todo: endian independant read of argv0*/
	if(sys_cx_bn_lock(ps_ctx->t8_modular, 0)!=CX_OK){
		return CY_KO;
	}

    if(sys_cx_bn_alloc_init( &bn_p, ps_ctx->t8_modular, argv[1], ps_ctx->t8_modular)!=CX_OK) /* allocating and initializing big number a*/
    {
    	error=CY_ERR_ALLOC;
    	goto end;
    }

    CY_CHECK(cy_mont_alloc ((cy_bn_mont_ctx_t *) ps_ctx->montgomery_ctx, ps_ctx->t8_modular));

    CY_CHECK(cy_mont_init((cy_bn_mont_ctx_t *) ps_ctx->montgomery_ctx, bn_p ));
    ps_ctx->offset=3*sizeof(bn_p);


    /* IV. Free*/
   	 sys_cx_bn_destroy(&bn_p);


    ps_ctx->is_initialized=CY_LIB_INITIALIZED;
	error=CY_OK;
	end:
	return error;
}


/* TODO: developp a container to import/export ciphered containers
cy_error_t wrap_bolos_fp_write(fp_ctx_t *ctx, uint8_t *Mem __attribute__((unused)), int argc, uint8_t **argv)
{


}


cy_error_t wrap_bolos_fp_read(fp_ctx_t *ctx, uint8_t *Mem __attribute__((unused)), int argc, uint8_t **argv)
{


}*/

cy_error_t wrap_bolos_fp_alloc(fp_ctx_t *ctx ,  size_t t8_r, cy_fp_t *r){
 cy_error_t error=CY_KO;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;
	 goto end;
 }

 r->ctx=ctx;
 r->Shared_Memory_index=(uint8_t *) (ctx->Shared_Memory+ctx->offset);
 r->bn=(cx_bn_t *) (ctx->Shared_Memory+ctx->offset);

 printf("\n %s allocating from %x with offset=%x to %x, t8_r=%d", ctx->libname, (unsigned int) ctx->Shared_Memory, ctx->offset, (unsigned int) r->Shared_Memory_index,(int) t8_r);

 CX_CHECK(sys_cx_bn_alloc(r->bn, t8_r));
 ctx->offset+=sizeof(cx_bn_t *);

 error=CY_OK;

 end:
  return error;
}

cy_error_t wrap_bolos_fp_free(cy_fp_t *r)
{
 fp_ctx_t *ctx=r->ctx;

 r->ctx=NULL;
 r->Shared_Memory_index=NULL;

 if(r->Shared_Memory_index==(uint8_t *) (ctx->Shared_Memory+ctx->offset)){
	 ctx->offset-=sizeof(cx_bn_t *);
 }
 return(sys_cx_bn_destroy(r->bn));
}

cy_error_t wrap_bolos_fp_import(  uint8_t *in, size_t t8_in, cy_fp_t *out){
	cy_error_t error=CY_KO;
	fp_ctx_t *ctx =out->ctx;

	 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
		 error=CY_ERR_UNIT;
		 goto end;
	 }

	 CX_CHECK(sys_cx_bn_init(*(out->bn), in, t8_in));

	 error=CY_OK;

	 end:
	  return error;

}


cy_error_t wrap_bolos_fp_add( cy_fp_t *a, cy_fp_t *b, cy_fp_t *out){
 cy_error_t error=CY_KO;
 fp_ctx_t *ctx =a->ctx;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;

	 goto end;
 }

 CX_CHECK(sys_cx_bn_mod_add(*(out->bn),*(a->bn),*(b->bn), *((cx_bn_t *) ctx->modular)));

 error=CY_OK;

 end:
  return error;
}


cy_error_t wrap_bolos_fp_to_mont(  cy_fp_t *in, cy_fp_t *out_mont){
 cy_error_t error=CY_KO;
 fp_ctx_t *ctx =in->ctx;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;

	 goto end;
 }

 CY_CHECK(cx_mont_to_montgomery(*(out_mont->bn),*(in->bn),  ctx->montgomery_ctx));

 error=CY_OK;

 end:
  return error;
}

cy_error_t wrap_bolos_fp_uninit(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory)
{
	cy_error_t error=CY_KO;

	size_t i;

	for(i=0;i<t8_Memory;i++){
		pu8_Mem[i]=0x55;}

	/* todo: add a mem checker, memory past offset shall be 0x55*/
	CX_CHECK(sys_cx_bn_unlock());

	UNUSED(ps_ctx);

	 end:
	  return error;
}




