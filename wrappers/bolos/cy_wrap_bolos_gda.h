/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_gda.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file */
/**********************************************************************************/

#ifndef CY_WRAP_BOLOS_GDA_H_
#define CY_WRAP_BOLOS_GDA_H_

#include "cy_gda_component.h"


/* Speculos wrapping necessary*/
#ifdef _WITH_SPECULOS
#define cx_rng_no_throw                                                        \
  (uint8_t * buffer, size_t len) cy_rng_no_throw(uint8_t *buffer, size_t len)
#endif

cy_error_t cy_rng_bolos_init(cy_gda_ctx_t *io_ps_ctx, uint8_t *initializer,
                             size_t initializer_t8);
cy_error_t cy_rng_bolos_run(cy_gda_ctx_t *io_ps_ctx, uint8_t *initializer,
                            size_t initializer_t8);
cy_error_t cy_rng_bolos_uninit(cy_gda_ctx_t *io_ps_ctx);

extern cy_gda_ctx_t bolos_gda_component;

#endif
