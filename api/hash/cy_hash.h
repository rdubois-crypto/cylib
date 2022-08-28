/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.*/
/* FILE: cy_errors.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: hash functions  API */
/**********************************************************************************/
#ifndef _CY_HASH_H
#define _CY_HASH_H


typedef cy_error_t (*Fct_Hash_Init_t)(void *io_ps_ctx, uint8_t *initializer,
                                     size_t initializer_t8);
typedef cy_error_t (*Fct_Hash_Update_t)(void *io_ps_ctx, uint8_t *out,
                                    size_t out_t8);
typedef cy_error_t (*Fct_Hash_Final_t)(void *io_ps_ctx);

struct hash_ctx_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  uint64_t id; /**< Component identifier                                   */
  unsigned char Component_name[MAX_SIZENAME];

  /*Arithmetic functions*/
  Fct_Hash_Init_t GDA_Init;
  Fct_Hash_Update_t GDA_Run; /**< pointer to run function                   */
  Fct_Hash_Final_t GDA_UnInit;

  cy_flag_t is_initialized;

  uint8_t *internal_state;
  size_t state_t8;

  void *opt;
  size_t opt_t8;
};

#define _SHA512_ID 1
#define _KECCAK256_ID 2


typedef struct hash_ctx_s cy_hash_ctx_t;

#endif
