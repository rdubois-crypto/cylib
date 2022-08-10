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
#include "innovation/cy_def.h"
#include "innovation/cy_errors.h"
#include "innovation/cy_memory_unit.h"
#include "innovation/cy_gda_component.h"


#ifndef _FP_UNIT_SELECTED
/************** CHOOSE YOUR FP UNIT ****/
/*Pick a burritos ingredient to be used for prime fields */
#define _FP_BOLOS_BURRITOS /* bolos*/
//#define _FP_LIB256K1_BURRITOS /*lib256k1zkp*/
//#define _FP_LIBECC_BURRITOS /* libecc*/
//#define _FP_CY_BURRITOS /* bolos*/
//#define _FP_ENCLAVE_BURRITOS /* bolos-enclave*/


#ifdef _FP_BOLOS_BURRITOS
#include <openssl/bn.h>
#include <openssl/ec.h>
#include "bolos/cx_ec.h"
#include "bolos/cxlib.h"
#include "innovation/cy_wrap_fp_bolos.h"

#define _FP_UNIT_SELECTED
#endif

#ifdef _FP_LIB256K1_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_lib256k1.h"
#define _FP_UNIT_SELECTED
#define _FP2_UNIT_DISABLED
#endif

#ifdef _FP_LIBECC_BURRITOS
#include "../../../src/innovation/cy_wrap_fp_libecc.h"
#endif

#ifdef _FP_CY_BURRITOS
#include "../../../src/innovation/cy_mod.h"
#endif

#include "innovation/cy_fp.h"
#include "innovation/cy_wrap_bolos_cryptolib.h"


#endif /* _FP_UNIT_SELECTED*/
/************** CHOOSE YOUR FP2 UNIT ****/
#ifndef _FP2_UNIT_DISABLED




#endif

#endif

