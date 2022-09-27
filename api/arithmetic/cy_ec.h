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


typedef struct cy_ec_s cy_ecpoint_t;


struct ec_ctx_s {
  cy_flag_t is_initialized;
  int curve_id;

  char libname[T8_MAX_LIBNAME];
  uint8_t *Shared_Memory;
  size_t offset;
  size_t max_offset;/* maximal allocated size*/
  size_t t8_modular_p;
  size_t t8_modular_q;
  cy_ecpoint_t *G;

  cy_fp_ctx_t *ctx_fp_p; /*pointer to modular context in Shared Memory*/
  //cy_fp_ctx_t *ctx_fp_q; /*pointer to modular context in Shared Memory*/
};

typedef struct ec_ctx_s cy_ec_ctx_t;


struct cy_ec_s{
  cy_inner_ec_t *ec;

  #ifndef _NO_CONTROL
  cy_flag_t is_initialized;
  #endif

  size_t index; /* offset to the object with the allocated Memory*/
  cy_ec_ctx_t *ctx;
};



#define _HANDLED_EC_MAX 16
#define _EC_ZONE_T8 (_HANDLED_EC_MAX*(sizeof(cy_ecpoint_t)*sizeof(cy_bn_t)))

/*
struct cy_ec_s {
  _COORD_HEADER;
  boolean_t is_normalized = 0;

  uint8_t *index;


  ec_ctx_t *ctx;
  void *external_type;
};

typedef struct cy_ec_s cy_ecpoint_t;
*/

extern cy_error_t cy_ec_init(cy_ec_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                             const size_t t8_Memory, const int argc,
                             const uint8_t *argv[]);

extern cy_error_t cy_ec_alloc(cy_ec_ctx_t *ps_ctx,  cy_ecpoint_t *out);
extern cy_error_t cy_ec_add(const cy_ecpoint_t *a, const cy_ecpoint_t *b,
		cy_ecpoint_t *r);

extern cy_error_t cy_ec_isoncurve(const cy_ecpoint_t *a, int *flag_verif);

extern cy_error_t cy_ec_scalarmult_fp(const cy_fp_t *k, const cy_ecpoint_t *P,  cy_ecpoint_t *R);

extern cy_error_t cy_ec_import( const uint8_t *xy, size_t t8_x, cy_ecpoint_t *G);
extern cy_error_t cy_ec_import2( uint8_t *x, size_t t8_x, uint8_t *y, size_t t8_y, cy_ecpoint_t *G);


extern cy_error_t cy_ec_export(const cy_ecpoint_t *G,  uint8_t *xy, size_t t8_xy);
extern cy_error_t cy_ec_export2(const cy_ecpoint_t *G,  uint8_t *x, size_t t8_x, uint8_t *y, size_t t8_y );
extern cy_error_t cy_ec_getX(const cy_ecpoint_t *a, cy_fp_t *out);

extern cy_error_t cy_ec_iseq(const cy_ecpoint_t *a, const cy_ecpoint_t *b, int *flag_verif);

extern cy_error_t cy_ec_scalarmult_MSBString(cy_ecpoint_t *a, cy_ecpoint_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_sub(cy_ecpoint_t *a, cy_ecpoint_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_scalarmul_bn(cy_ecpoint_t *a, cy_bn_t *b, cy_ecpoint_t *r);

extern cy_error_t cy_ec_free(cy_ecpoint_t *ec);

extern cy_error_t cy_ec_get_generator(const cy_ec_ctx_t *ec_ctx, cy_ecpoint_t *G);
extern cy_error_t cy_ec_uninit(cy_ec_ctx_t *ctx);

/* max bolos supported curve size is BLS12:384 bits*/
#define MAX_BOLOS_EC_T8 48

#endif /* SRC_INNOVATION_CY_EC_H_ */
