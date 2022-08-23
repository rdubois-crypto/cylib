/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: lib256k1 wrappers for cy_lib */
/**********************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "cy_def.h"
#include "cy_errors.h"

#include "external/libsec256k1/secp256k1.h"

#include "external/libsec256k1/libsec256k1_field_10x26.h"
#include "external/libsec256k1/libsec256k1_field_10x26_impl.h"
#include "external/libsec256k1/libsec256k1_field_impl.h"

#include "cy_wrap_fp_lib256k1.h"
#include "cy_fp.h"

#define word32_from_be(a) (a[0] + (a[1] << 8) + (a[2] << 16) + (a[3] << 24))

cy_error_t wrap_lib256k1_fp_init(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                                 const size_t t8_Memory, const int argc,
                                 const uint8_t *argv[])
{

  cy_error_t error = CY_KO;

  size_t i;
  size_t offset = 0;

  if (argc != 2) {
    return CY_KO;
  }
  /*control memory limit*/
  if (t8_Memory > _MAX_MEMORY) {
    error = CY_KO;
    goto end;
  }

  for(i=0;i<t8_Memory;i++) pu8_Mem[i]=_MEM_FP_RESERVED;

  ps_ctx->Shared_Memory = pu8_Mem; /*set Memory*/

  strcpy(ps_ctx->libname, LIB256K1_LIBNAME);
  ps_ctx->is_initialized = CY_LIB_INITIALIZED;

  ps_ctx->t8_modular = word32_from_be(argv[0]);
  for (i = 0; i < ps_ctx->t8_modular; i++)
    pu8_Mem[i + offset] = argv[1][i];
  offset += i;

  ps_ctx->modular = (void *)ps_ctx->Shared_Memory;
  ps_ctx->offset = ps_ctx->t8_modular;


  ps_ctx->is_initialized = CY_LIB_INITIALIZED;
  error = CY_OK;

  UNUSED(secp256k1_fe_is_quad_var);
  UNUSED(secp256k1_fe_inv_var);
  UNUSED(secp256k1_fe_inv);
  UNUSED(secp256k1_fe_to_storage);
  UNUSED(secp256k1_fe_get_b32);
  UNUSED(secp256k1_fe_set_b32);
  UNUSED(secp256k1_fe_cmp_var);
  UNUSED(secp256k1_fe_cmp_var);
  UNUSED(secp256k1_fe_cmp_var);
  UNUSED(secp256k1_fe_normalize_weak);
  UNUSED(secp256k1_fe_get_bounds);

end:
  return error;
}

cy_error_t wrap_lib256k1_fp_alloc(cy_fp_ctx_t *ctx, size_t t8_r, cy_fp_t *r)
{
  cy_error_t error = CY_KO;
  size_t i;

  /* lib256k1 only accept 32 bytes as input size*/
  if (t8_r != 32){
 	error=CY_ERR_LENGTH;
 	goto end;
   }
  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    goto end;
  }


  r->ctx=ctx;
  r->is_initialized=CY_LIB_INITIALIZED;
  r->index =  ctx->offset;
  r->bn = (cy_bn_t *)(ctx->Shared_Memory + ctx->offset);

  for(i=ctx->offset;i<(ctx->offset+sizeof(cy_bn_t ));i++)
  {
	  ctx->Shared_Memory[i]=_MEM_FP_ALLOCATED;
  }

  ctx->offset += sizeof(cy_bn_t );

  error = CY_OK;

end:
  return error;
}


cy_error_t wrap_lib256k1_fp_free(cy_fp_t *r)
{
	 cy_error_t error = CY_KO;
     size_t i;
     cy_fp_ctx_t *ctx = r->ctx;

	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;
	    goto end;
	  }

	  for(i=0;i<sizeof(cy_bn_t);i++)
	    {
	  	  *(ctx->Shared_Memory+r->index+i)=_MEM_FP_RESERVED;
	    }

	    r->index = CY_LIB_UNINITIALIZED;
	    r->ctx = NULL;

	    if (r->index ==  ctx->offset) {
	      ctx->offset -= sizeof(cy_bn_t);
	    }


	 ctx->offset -= sizeof(cy_bn_t );

	  error = CY_OK;

	end:
	  return error;
}


cy_error_t wrap_lib256k1_fp_import(const uint8_t *in, size_t t8_in, cy_fp_t *out)
{
  cy_error_t error = CY_OK;
  cy_fp_ctx_t *ctx = out->ctx;

  size_t i;


  if (ctx->is_initialized != CY_LIB_INITIALIZED) {

	error = CY_ERR_UNIT;
    goto end;
  }

  if (t8_in != _LIB256K1_FE_T8){
	error=CY_ERR_LENGTH;

	goto end;
  }
  secp256k1_fe_storage cin;


  for (i = 0; i <(t8_in>>2) ; i++) {
    (cin.n)[i] = word32_from_be((in + (i << 3)));
  }

  (secp256k1_fe_from_storage(out->bn, &cin));
  UNUSED(cin);

  error = CY_OK;

end:
  return error;
}

cy_error_t wrap_lib256k1_fp_export(const cy_fp_t *in, uint8_t *out, size_t t8_in)
{
  cy_error_t error = CY_OK;
  cy_fp_ctx_t *ctx = in->ctx;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {

	error = CY_ERR_UNIT;
    goto end;
  }

  if (t8_in != _LIB256K1_FE_T8){
	error=CY_ERR_LENGTH;

	goto end;
  }
  secp256k1_fe_storage storage_out;

  /* dangerous cast, TODO make proper conversion*/
//  storage_out.n=(uint32_t[8]) out;

  (secp256k1_fe_to_storage(&storage_out, in->bn));


  UNUSED(out);
  error = CY_OK;

end:
  return error;
}

cy_error_t wrap_lib256k1_fp_copy(const cy_fp_t *in, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
		  cy_fp_ctx_t *ctx = in->ctx;

		  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
		    error = CY_ERR_UNIT;

		    goto end;
		  }

		  (secp256k1_fe_cmov((out->bn), (in->bn), 1));
		  error=CY_OK;

		end:
		  return error;

}


cy_error_t wrap_lib256k1_fp_set_zero( cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
			  cy_fp_ctx_t *ctx = out->ctx;

			  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
			    error = CY_ERR_UNIT;

			    goto end;
			  }

			  secp256k1_fe_set_int(out->bn,  0);
			  error=CY_OK;

			end:
			  return error;

}


cy_error_t wrap_lib256k1_fp_sqr(const cy_fp_t *in, cy_fp_t *out)
{
	  cy_fp_ctx_t *ctx = in->ctx;
	  cy_error_t error = CY_OK;

		  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
		    error = CY_ERR_UNIT;

		    goto end;
		  }

		  (secp256k1_fe_sqr((out->bn), (in->bn)));
		  error=CY_OK;

		end:
		  return error;
}

cy_error_t wrap_lib256k1_fp_inc(cy_fp_t *in, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = in->ctx;

	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;

	    goto end;
	  }

	  (secp256k1_fe_add((out->bn), (in->bn)));
	  error=CY_OK;

	end:
	  return error;
}

cy_error_t wrap_lib256k1_fp_add(cy_fp_t *a, cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = out->ctx;



  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    printf("\n not initialized");

    goto end;
   }

  secp256k1_fe_cmov(out->bn, a->bn, CY_TRUE);

  CY_CHECK(wrap_lib256k1_fp_inc(b, out));

end:
  return error;
}


cy_error_t wrap_lib256k1_fp_mul(cy_fp_t *a, cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = out->ctx;



  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    printf("\n not initialized");

    goto end;
   }

  secp256k1_fe_mul(out->bn, a->bn, b->bn);

  CY_CHECK(wrap_lib256k1_fp_inc(b, out));

end:
  return error;
}


cy_error_t wrap_lib256k1_fp_inv(cy_fp_t *in, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = in->ctx;

	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;

	    goto end;
	  }

	  (secp256k1_fe_inv((out->bn), (in->bn)));
	  error=CY_OK;

	end:
	  return error;
}


/*secp256k1_fe_negate(secp256k1_fe *r,
                                                 const secp256k1_fe *a, int m)*/

#define _MAX_MAGNITUDE (1<<30)

cy_error_t wrap_lib256k1_fp_sub(cy_fp_t *a, cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = out->ctx;



  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    printf("\n not initialized");

    goto end;
   }

  /* a-b = p+b+a */
  secp256k1_fe_negate(out->bn, b->bn, _MAX_MAGNITUDE);
  CY_CHECK(wrap_lib256k1_fp_inc(a, out));

end:
  return error;
}

//void secp256k1_fe_mul(secp256k1_fe *r, const secp256k1_fe *a,                             const secp256k1_fe *SECP256K1_RESTRICT b);



cy_error_t wrap_lib256k1_fp_neg(cy_fp_t *b, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = out->ctx;

	  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
	    error = CY_ERR_UNIT;
	    printf("\n not initialized");

	    goto end;
	   }
	  secp256k1_fe_negate(out->bn, b->bn, _MAX_MAGNITUDE);


	end:
	  return error;
}

/* for now we handle montgomery by cheating it to be identical to normalized version */
cy_error_t wrap_lib256k1_fp_mult_mont( cy_fp_t *a, cy_fp_t *b, cy_fp_t *out){

	return(wrap_lib256k1_fp_mul(a,b,out));
}

cy_error_t wrap_lib256k1_fp_uninit(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                                const size_t t8_Memory)
{
  cy_error_t error = CY_KO;

  size_t i;
  /* from here, if correctly used, the only element remaining is the modulus stored at first address*/
  for(i=0;i<_LIB256K1_FE_T8;i++){
	  pu8_Mem[i]=_MEM_FP_RESERVED;
  }

  for(i=0;i<t8_Memory;i++){
	  if( (pu8_Mem[i])!=(uint8_t) _MEM_FP_RESERVED)
	  {
		  error=CY_MEM_FREE_MISSING;
	  goto end;
	  }
  }
  UNUSED(ps_ctx);
  error = CY_OK;
end:
  return error;
}


/*****************************************************************************/
/*	II. Access to private fields methods
 */
/*****************************************************************************/
/* no montgomery reduction with lib256k1, as a Solinas sparse reduction is used */
cy_bn_t* wrap_lib256k1_get_fp_montgomery_constant1(cy_fp_t *in)
{
		  UNUSED(in);
		  return NULL;
}


