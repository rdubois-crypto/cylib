/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_nist_cavp.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: process the nist CAVP vectors, see description in  */
/*https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program*/
/**********************************************************************************/
#include <stdio.h> /* required by fopen*/
#include "cy_errors.h"

extern cy_error_t cy_NIST_CAVP_process(FILE *f_in, FILE *f_out);

