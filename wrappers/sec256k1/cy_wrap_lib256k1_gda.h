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

#ifndef CY_WRAP_LIB256K1_H_
#define CY_WRAP_LIB256K1_H_

#include "cy_gda_component.h"


cy_error_t cy_rng_lib256k1_init(cy_gda_ctx_t *io_ps_ctx, uint8_t *initializer,
                             size_t initializer_t8);
cy_error_t cy_rng_lib256k1_run(cy_gda_ctx_t *io_ps_ctx, uint8_t *initializer,
                            size_t initializer_t8);
cy_error_t cy_rng_lib256k1_uninit(cy_gda_ctx_t *io_ps_ctx);

extern cy_gda_ctx_t lib256k1_gda_component;

#endif
