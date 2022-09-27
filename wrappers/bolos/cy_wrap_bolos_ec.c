/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_ec.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib elliptic curves arithmetic */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "bolos/cxlib.h"
#include "cy_errors.h"


#include "cy_wrap_fp_bolos.h"


#include "cy_fp.h"

#include "cy_wrap_bolos_ec.h"

#include "cy_ec.h"
//#include "cy_ec_const.h"

/*****************************************************************************/
/*	I. completing missing Speculos functions
 */
/*****************************************************************************/

#define  cy_ecdomain_parameters_length(curve, length) sys_cx_ecdomain_parameters_length(curve,length);


cy_error_t cy_ecpoint_add	(	cx_ecpoint_t * 	out,
const cx_ecpoint_t * 	P,
const cx_ecpoint_t * 	Q
)
{
	cx_curve_t curve=P->curve;
    size_t len;
    cy_ecdomain_parameters_length(curve, &len);

    unsigned char bs_P[3*MAX_BOLOS_EC_T8] ;
    unsigned char bs_Q[3*MAX_BOLOS_EC_T8] ;
    unsigned char bs_R[3*MAX_BOLOS_EC_T8] ;

    sys_cx_ecpoint_export(P, bs_P, len,
    		bs_P+len, len);

    sys_cx_ecpoint_export(Q, bs_Q, len,
        		bs_Q+len, len);



	UNUSED(out);
	UNUSED(P);
	UNUSED(Q);
	sys_cx_ecfp_add_point(curve, bs_R, bs_P,bs_Q, len);



    return CY_KO;
}

/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling
 */
/*****************************************************************************/

/* the mapping of memory in bolos for dual FP/EC units is
 * fp_ctx is written at adress 0
 * fp offset counter is used to stack both fp and ec
 * fp_uninit erase all pointers
 * ec_uninit erase the fp ctx
 */
static cy_error_t inc_offset(cy_ec_ctx_t *ec_ctx, size_t t8_inc)
{
	size_t total_offset;

	ec_ctx->ctx_fp_p->offset+=t8_inc;
	total_offset=ec_ctx->ctx_fp_p->offset+ec_ctx->offset;

	if(total_offset > ec_ctx->max_offset) return CY_MEM_OVERFLOW;

	return CY_OK;
}

/* argc input is the value of the curve, argv is  NULL*/
cy_error_t wrap_bolos_ec_init(cy_ec_ctx_t *ec_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory, const int argc,
                              const uint8_t *argv[])
{
  UNUSED(argv);

  cy_error_t error = CY_KO;
  //cx_curve_t curve = (cx_curve_t) (argv[0]);
  cx_curve_t curve = (cx_curve_t) argc;



  size_t size_param_t8=0;/* byte size of parameters */
  const uint8_t *argv_gen[]={NULL, NULL};

  unsigned char argv_prime[MAX_BOLOS_EC_T8+4] ;


  if (cx_ecdomain(curve) == NULL)
    return CY_UNHANDLED_CURVE;


  CY_CHECK(cy_ecdomain_parameters_length(curve, & size_param_t8));

  ec_ctx->offset = 0;

  (ec_ctx->Shared_Memory) = pu8_Mem;

  ec_ctx->ctx_fp_p=(cy_fp_ctx_t *) pu8_Mem;

  ec_ctx->offset = sizeof(cy_fp_ctx_t );

//  ec_ctx->offset +=sizeof(cy_fp_ctx_t *);

  be_from_word32(size_param_t8, argv_prime);
  ec_ctx->curve_id = curve;

  sys_cx_ecdomain_parameter(curve, CX_CURVE_PARAM_Field, argv_prime+4, (uint32_t) size_param_t8);


  argv_gen[0]=argv_prime;
  argv_gen[1]=argv_prime+4;


  CY_CHECK(wrap_bolos_fp_init(ec_ctx->ctx_fp_p, pu8_Mem+ec_ctx->offset, t8_Memory-ec_ctx->offset, 2, argv_gen));

  strcpy(ec_ctx->libname, BOLOS_EC_LIBNAME);


  ec_ctx->is_initialized = CY_LIB_INITIALIZED;

 // printf("\n here with code=%x", (unsigned int) error);

 // wrap_bolos_fp_init(ec_ctx->ctx_fp_q, pu8_Mem, t8_Memory, 2, argv_gen);
 // ec_ctx->offset += ec_ctx->ctx_fp_q->offset;


  end:
  	return error;
}

cy_error_t wrap_bolos_ec_uninit(cy_ec_ctx_t *ec_ctx)
{
	size_t i;

    cy_error_t error = CY_KO;
	cy_fp_ctx_t *fp_ctx=ec_ctx->ctx_fp_p;
	for(i=0;i<sizeof(cy_fp_ctx_t *);i++)
		ec_ctx->Shared_Memory[i]=_MEM_FP_RESERVED;

	CY_CHECK(wrap_bolos_fp_uninit(fp_ctx, fp_ctx->Shared_Memory,fp_ctx->offset));

	 end:
		return error;
}


cy_error_t wrap_ecpoint_alloc( cy_ec_ctx_t *ec_ctx, cy_ecpoint_t *P)
{
  cy_error_t error = CY_KO;
  P->ctx = ec_ctx;

  P->index =  ec_ctx->offset;

  if (ec_ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    goto end;

  }

  P->ec = (cx_ecpoint_t *)(ec_ctx->Shared_Memory + ec_ctx->offset+ec_ctx->ctx_fp_p->offset);

  CY_CHECK(sys_cx_ecpoint_alloc (P->ec, ec_ctx->curve_id));

  inc_offset( ec_ctx, sizeof(cy_inner_ec_t ));



  end:
     return error;
}

cy_error_t wrap_bolos_ec_add(const cy_ecpoint_t * a, const cy_ecpoint_t * b,
                               cy_ecpoint_t * out)
  {
    cy_error_t error = CY_KO;
    cy_ec_ctx_t *ctx = a->ctx;

    if (ctx->is_initialized != CY_LIB_INITIALIZED) {
      error = CY_ERR_UNIT;

      goto end;
    }

     CX_CHECK(sys_cx_ecpoint_add(out->ec, a->ec, b->ec));


  end:
    return error;
  }

cy_error_t cy_ec_iseq(const cy_ecpoint_t *a, const cy_ecpoint_t *b, int *flag_verif)
{
	cy_error_t error = CY_KO;

	CY_CHECK(sys_cx_ecpoint_cmp(a->ec,b->ec, (bool*) flag_verif));
	end:
	  return error;
}

cy_error_t wrap_bolos_ec_import(const uint8_t *xy, size_t t8_x, cy_ecpoint_t *G){
	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecpoint_init(G->ec, xy,t8_x, xy+t8_x, t8_x));

	end:
		    return error;
}


cy_error_t wrap_bolos_ec_import2(uint8_t *x, size_t t8_x, uint8_t *y, size_t t8_y, cy_ecpoint_t *G){

	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecpoint_init(G->ec, x,t8_x, y, t8_y));

	end:
		    return error;
}

cy_error_t wrap_bolos_ec_export(const cy_ecpoint_t *G,  uint8_t *xy, size_t t8_x)
{
	 cy_error_t error = CY_KO;

	 CY_CHECK(sys_cx_ecpoint_export(G->ec, xy, t8_x, xy+t8_x, t8_x));

	 end:
	  	return error;
}


cy_error_t wrap_bolos_get_generator(const cy_ec_ctx_t *ec_ctx, cy_ecpoint_t *G){

	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecdomain_generator_bn(ec_ctx->curve_id, G->ec));

	  end:
	    return error;

}

cy_error_t wrap_bolos_cy_ec_copy(const cy_ecpoint_t * P_in, cy_ecpoint_t *P_out)
{
	 cy_error_t error = CY_KO;

	 CY_CHECK(sys_cx_bn_copy(P_out->ec->x, P_in->ec->x));
	 CY_CHECK(sys_cx_bn_copy(P_out->ec->y, P_in->ec->y));
	 CY_CHECK(sys_cx_bn_copy(P_out->ec->z, P_in->ec->z));

	 end:
	 	 return error;
}

cy_error_t wrap_bolos_ec_scalarmul_fp(const cy_fp_t * k, const cy_ecpoint_t * P,
                               cy_ecpoint_t *kP)
  {
    cy_error_t error = CY_KO;

    CY_CHECK(wrap_bolos_cy_ec_copy(P, kP));

    CY_CHECK(sys_cx_ecpoint_scalarmul_bn(kP->ec, *k->bn));

    end:
    	return error;
  }

cy_error_t wrap_bolos_getX(const cy_ecpoint_t *a, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;

	 CY_CHECK(sys_cx_bn_copy( *out->bn, a->ec->x));



	 end:
		    	return error;

}

cy_error_t wrap_bolos_isoncurve(const cy_ecpoint_t *a, int *flag_verif)
{
	  cy_error_t error = CY_KO;


	    CY_CHECK(sys_cx_ecpoint_is_on_curve(a->ec, (bool*) flag_verif));

	    end:
	    	return error;

}


cy_error_t wrap_ecpoint_free(cy_ecpoint_t *P)
{
  size_t i;
  cy_error_t error = CY_OK;
  cy_ec_ctx_t *ec_ctx= ec_ctx;

  CY_CHECK(sys_cx_ecpoint_destroy (P->ec));

  for(i=0;i<sizeof(cy_ecpoint_t *);i++)
    {
	  ((uint8_t *) P)[i]=_MEM_EC_RESERVED;
    }


  end:
     return error;
}


