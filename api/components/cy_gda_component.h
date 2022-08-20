/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_arithmetic_component.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: defining arithmetic component in an object-C way */
/* this structure can be used for rust bindings
 */
/**********************************************************************************/

#ifndef CY_GDA_COMPONENT_H_
#define CY_GDA_COMPONENT_H_

typedef cy_error_t (*Fct_Gda_Init_t)(void *io_ps_ctx, uint8_t *initializer,
                                     size_t initializer_t8);
typedef cy_error_t (*Fct_Gda_Run_t)(void *io_ps_ctx, uint8_t *out,
                                    size_t out_t8);
typedef cy_error_t (*Fct_Gda_UnInit_t)(void *io_ps_ctx);

struct gda_ctx_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  uint64_t id; /**< Component identifier                                   */
  unsigned char Component_name[MAX_SIZENAME];

  /*Arithmetic functions*/
  Fct_Gda_Init_t GDA_Init;
  Fct_Gda_Run_t GDA_Run; /**< pointer to run function                   */
  Fct_Gda_UnInit_t GDA_UnInit;

  cy_flag_t is_initialized;

  uint8_t *internal_state;
  size_t state_t8;

  void *opt;
  size_t opt_t8;
};

typedef struct gda_ctx_s cy_gda_ctx_t;

#endif /* SRC_OBJECTC_CY_GDA_COMPONENT_H_ */
