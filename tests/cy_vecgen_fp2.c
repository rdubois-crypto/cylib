/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_vecgen_wrap_fp2.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: generating tests vectors for quadratic arithmetic */
/**********************************************************************************/



#include "cy_ec_const.h"
#include "cy_errors.h"
#include "cy_fp.h"

#include "cy_quad.h"
#include "cy_fp2.h"



/* note that only stark and bls12 support quadratic extensions with X2+1=0 */
static cy_error_t vecgen_fp2_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;

	printf("\n\n /************************ Vecgen for Quadratic Extension Unit:");

	#ifdef _FP2_UNIT_DISABLED
	printf("\n <not implemented with this FP Unit> : skipped : OK");
	return CY_OK;
 	#endif

	printf("\n nb supported modules=%d", _QUADRATIC_FRIENDLY_END);

	// const uint8_t *argv_bls381[] = { secp384r1_t8, C_cx_BLS12_381_G1_p };
	 //const uint8_t *argv_sec384[] = { secp384r1_t8, C_cx_secp384r1_p };
	 //C_cx_secp384r1->argv,
    const uint8_t *argv_gen[]={NULL, NULL};



	for(i=0;i<_QUADRATIC_FRIENDLY_END;i++){

	    argv_gen[0]=C_cx_allCurves[i]->t8_size;
	    argv_gen[1]=C_cx_allCurves[i]->p;

	 CY_CHECK(vecgen_fp2_crypto_parameters(argv_gen, 2, C_cx_allCurves[i]->curve_name, Ramp, Ramp_t8));
	}


	end:
	  return error;
}

