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

#ifndef API_CY_EC_H_
#define API_CY_EC_H_

struct ec_ctx_s {
  cy_flag_t is_initialized;
  int curve_id;

  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;

  size_t t8_modular_p;
  size_t t8_modular_q;

  cy_fp_ctx_t ctx_fp_p; /*pointer to modular context in Shared Memory*/
  cy_fp_ctx_t ctx_fp_q; /*pointer to modular context in Shared Memory*/
};

typedef struct ec_ctx_s ec_ctx_t;

struct cy_ec_s {
  _COORD_HEADER;
  boolean_t is_normalized = 0;

  uint8_t *index;


  ec_ctx_t *ctx;
  void *external_type;
};

typedef struct cy_ec_s cy_ecpoint_t;

extern cy_error_t cy_ec_init(ec_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                             const size_t t8_Memory, const int argc,
                             const uint8_t *argv[]);

extern cy_error_t cy_ec_alloc(ec_ctx_t *ps_ctx, size_t ec_t8, cy_ec_t *out);
extern cy_error_t cy_ec_import(ec_ctx_t *ps_ctx, uint8_t *in, size_t ec_t8,
                               cy_ec_t *out);
extern cy_error_t cy_ec_add(ec_ctx_t *ctx, cy_ecpoint_t *a, cy_ec_t *b,
		cy_ecpoint_t *r);

extern cy_error_t cy_ec_scalarmult_fp( cy_ecpoint_t *P, cy_fp_t *k, cy_ecpoint_t *R);

extern cy_error_t cy_ec_scalarmult_MSBString(cy_ecpoint_t *a, cy_ecpoint_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_sub(cy_ecpoint_t *a, cy_ecpoint_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_scalarmul_bn(cy_ecpoint_t *a, cy_bn_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_free(cy_ecpoint_t *ec);

extern cy_error_t cy_ec_uninit(ec_ctx_t *ctx, uint8_t *pu8_Mem,
                               const size_t t8_Memory);

#endif /* SRC_INNOVATION_CY_EC_H_ */
