/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_configuration.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION:  This file is intended to provide a description of the retained
 * wrappers for the CY_LIB configuration choice */
/**********************************************************************************/


#ifndef _CY_CONFIG_H
#define _CY_CONFIG_H
/* Common to all implementations */
#include "cy_def.h"
#include "cy_errors.h"
#include "cy_memory_unit.h"
#include "cy_gda_component.h"

/************** CONFIGURATION OF THE LIBRARY ****/
/* The following compilation options enable the selection of which
 * library/cryptographic component is used for each service. Those services contains:
 * - gda/trng (physical true random number generator)
 * - fp : modular integers
 * - fp2: quadratic extensions
 * - fp12: dodecaic extensions
 * - ec: elliptic curves computations
 */


/************** CHOOSE YOUR GDA (TRNG) UNIT ****/
#define _GDA_BOLOS_BURRITOS

/************** CHOOSE YOUR FP UNIT ****/
/*Pick a burritos ingredient to be used for prime fields */
#define _FP_BOLOS_BURRITOS /* bolos*/
//#define _FP_LIB256K1_BURRITOS /*lib256k1zkp*/
//#define _FP_LIBECC_BURRITOS /* libecc*/
//#define _FP_CY_BURRITOS /* bolos*/
//#define _FP_ENCLAVE_BURRITOS /* bolos-enclave*/

/************** CHOOSE YOUR FP2 UNIT ****/
#define _FP2_CY_BURRITOS

/************** CHOOSE YOUR FP12 UNIT ****/
#define _FP12_CY_BURRITOS

/************** CHOOSE YOUR EC UNIT ****/
#define _EC_BOLOS_BURRITOS


#ifdef _FP_BOLOS_BURRITOS
#include <openssl/bn.h>
#include <openssl/ec.h>
#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#include "../wrappers/bolos/cy_wrap_fp_bolos.h"
#include "../wrappers/bolos/cy_wrap_bolos_bn.h"

#define _FP_UNIT_SELECTED
#endif

#ifdef _FP_LIB256K1_BURRITOS
#include "../wrappers/libsec256k1/cy_wrap_fp_lib256k1.h"
#define _FP_UNIT_SELECTED
#define _FP2_UNIT_DISABLED
#endif

#ifdef _FP_LIBECC_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_libecc.h"
#define _FP_UNIT_SELECTED
#endif

#ifdef _FP_CY_BURRITOS
#include "../../../src/innovation/cy_mod.h"
#define _FP_UNIT_SELECTED
#endif

#include "cy_fp.h"


//#endif /* _FP_UNIT_SELECTED*/
/************** CHOOSE YOUR FP2 UNIT ****/
#ifndef _FP2_UNIT_DISABLED

#define _FP2_CY_BURRITOS
#ifdef _FP2_CY_BURRITOS
#include "cy_quad.h"
#define _FP2_UNIT_SELECTED
#endif

#include "cy_fp2.h"


/************** CHOOSE YOUR FP12 UNIT ****/

#ifdef _FP12_CY_BURRITOS
#include "cy_fp2x3x2.h"
#define _FP12_UNIT_SELECTED
#endif

#endif /*endif _FP2_UNIT_DISABLED*/

#include "../wrappers/bolos/cy_wrap_bolos_cryptolib.h"


/************** CHOOSE YOUR EC UNIT ****/
#ifdef _EC_BOLOS_BURRITOS
#include "../wrappers/bolos/cy_wrap_bolos_ec.h"
#endif

#endif


/************** CHOOSE YOUR GDA UNIT ****/


#ifdef _GDA_BOLOS_BURRITOS
 #include "../wrappers/bolos/cy_wrap_bolos_gda.h"
#endif



