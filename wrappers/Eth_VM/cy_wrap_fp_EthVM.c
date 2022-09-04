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
/* DESCRIPTION: libECC wrappers for cy_lib prime fied arithmetic */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "cy_def.h"
#include "cy_errors.h"

#include "cy_wrap_fp_libECC.h"

#include "external/libECC/words/words.h"
#include "external/libECC/nn/nn.h"
#include "external/libECC/fp/fp.h"


#include "cy_fp.h"

/* it is reasonnable to assume that the precomputations will be stored directly */
cy_error_t wrap_EthVM_fp_init(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
        const size_t t8_Memory, const int argc,
        const uint8_t *argv[])
{
	cy_error_t error=wrap_bolos_fp_init(ps_ctx, pu8_Mem,
	                              t8_Memory,  argc,
	                              argv);


}

