/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_ec.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Elliptic curves  API */
/**********************************************************************************/

#ifndef API_CY_ECFP2_H_
#define API_CY_ECFP2_H_

struct ecfp2_ctx_s {
  cy_flag_t is_initialized;
  int curve_id;

  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;

  size_t t8_modular_p;
  size_t t8_modular_q;

  void *modular_p; /*pointer to modular context in Shared Memory*/
  void *modular_q; /*pointer to modular context in Shared Memory*/
};

typedef struct ecfp2_ctx_s ecfp2_ctx_t;

struct cy_ecfp2_s {
 _COORD_HEADER; /* the coordonates are defined by the EC_UNIT used*/
  boolean_t is_normalized = 0;

  uint8_t *index;

  ecfp2_ctx_t *ctx;
};


typedef struct cy_ecfp2_s cy_ecpoint_t;

extern cy_error_t cy_ecfp2_init(ecfp2_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                             const size_t t8_Memory, const int argc,
                             const uint8_t *argv[]);

extern cy_error_t cy_ecfp2_alloc(ecfp2_ctx_t *ps_ctx, size_t ecfp2_t8, cy_ecfp2_t *out);
extern cy_error_t cy_ecfp2_import(ecfp2_ctx_t *ps_ctx, uint8_t *in, size_t ecfp2_t8,
                               cy_ecfp2_t *out);
extern cy_error_t cy_ecfp2_add(ecfp2_ctx_t *ctx, cy_ecpoint_t *a, cy_ecfp2_t *b,
                            cy_ecfp2_t *r);

extern cy_error_t cy_ecfp2_sub(ecfp2_ctx_t *ctx, cy_ecfp2_t *a, cy_ecfp2_t *b, cy_ecfp2_t *r);

extern cy_error_t cy_ecfp2_scalarmul_bn(cy_ecfp2_t *a, cy_bn_t *b, cy_ecfp2_t *r);

extern cy_error_t cy_ecfp2_free(cy_ecfp2_t *ec);

extern cy_error_t cy_ecfp2_uninit(ecfp2_ctx_t *ctx, uint8_t *pu8_Mem,
                               const size_t t8_Memory);

#endif /* SRC_INNOVATION_CY_ECFP2_H_ */
