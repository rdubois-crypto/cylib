/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_blst.h*/
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Library initialization and management API */
/**********************************************************************************/

#ifndef SRC_INNOVATION_CY_WRAP_BLST_H_
#define SRC_INNOVATION_CY_WRAP_BLST_H_

struct cy_fp_ctx_s {
  limb_t *modular;
  size_t t8_modular;
  char libname[] = "blst";
};

typedef struct cy_fp_ctx_s cy_fp_ctx_t;

/* Wrapping blst functions to cylib*/
#define cy_fp_add(c, a, b, r)       wrap_blst_fp_add(c, a, b, r)
#define cy_fp_sub(c, a, b, r)       wrap_blst_fp_add(c, a, b, r)
#define cy_fp_mult_mont(c, a, b, r) wrap_blst_fp_mult_mont(c, a, b, r)

#endif /* SRC_INNOVATION_CY_WRAP_BLST_H_ */
