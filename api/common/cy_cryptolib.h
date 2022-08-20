/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_lib.h*/
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Library initialization and management API */
/**********************************************************************************/

#ifndef CY_CRYPTOLIB_H_
#define CY_CRYPTOLIB_H_

#include "cy_def.h"
#include "cy_gda_component.h"

/* in case of any error, this check will close the library, safely erasing the working memory*/
#ifdef _RELEASE
#define CY_LIBCHECK(call)                                                         \
  do {                                                                         \
    error = call;                                                              \
    if (error) {                                                               \
      cy_lib_uninit(cryptolib)\
      goto end;                                                                \
    }                                                                          \
  } while (0)
#else
#define CY_LIBCHECK(call) CY_CHECK(call)
#endif


struct cryptolib_ctx_s {
  uint8_t *Shared_Memory;
  unsigned char Cryptolib_name[MAX_SIZENAME]; /*library name*/

  cy_mem_ctx_t *mem_unit; /* memory unit manager of the cryptolib*/
  cy_gda_ctx_t *gda;      /* gda component of the cryptolib */

  /* to be discussed: do we want cryptographic components ?*/
  /*cy_arithm_Field_Component_t*/
  /*cy_cipher_component_t */
  /*cy_hash_component_t */
  /*cy_hmac_component_t */

  /* in operational mode this flag shall be reset in case of any error*/
  cy_flag_t is_initialized;

  uint8_t *reserved; /*reserved for more advanced memory control*/
};

typedef struct cryptolib_ctx_s cryptolib_ctx_t;

extern cy_error_t cy_lib_init(cryptolib_ctx_t *i_cryptolib,
                              uint8_t *Shared_Memory, size_t allocated_t8,
                              uint8_t *additional, size_t additional_t8);
extern cy_error_t cy_lib_check(const cryptolib_ctx_t *i_cryptolib);
extern cy_error_t cy_lib_uninit(cryptolib_ctx_t *i_cryptolib);

#endif
