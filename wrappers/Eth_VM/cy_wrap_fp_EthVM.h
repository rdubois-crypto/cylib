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

#include "wrappers/libsec256k1/cy_wrap_fp_lib256k1.c"
#include "cy_fp.h"

