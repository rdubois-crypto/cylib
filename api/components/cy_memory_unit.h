/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_memory_unit.h */
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Memory unit emulation API */
/**********************************************************************************/

#ifndef MEM_CY_H_
#define MEM_CY_H_

#define T8_MAX_LIBNAME                                                         \
  32 /* maximal size of the library name (example: "bolos_v1.0.O")*/
#define T8_MAX_OBJECTNAME                                                      \
  32 /* maximal size of object name (example : "modular_ari_component")*/
#define _MAX_MEMORY 5120 /* max bolos size app is around 4ko=(1<<17)*/

struct mem_ctx_s {
  uint8_t *Shared_Memory;
  size_t allocated_t8; // total size allocated to the library
  size_t alignment_t8; // minimal size of allocation for alignment, can be
                       // 8,16,32,64,128, 256
  size_t offset; // current offset in bytes from starting stack
  cy_flag_t is_initialized;


  uint8_t *reserved; /*reserved for more advanced memory control*/

};

typedef struct mem_ctx_s cy_mem_ctx_t;

extern cy_error_t cy_mem_init(cy_mem_ctx_t *io_mem, size_t allocated_t8,
                              size_t alignment_t8);
extern cy_error_t cy_mem_malloc(cy_mem_ctx_t *io_mem, size_t allocated_t8, uint8_t **index_out);
extern cy_error_t cy_mem_free(cy_mem_ctx_t *io_mem, uint8_t *index,
                              size_t sizeof_element);
extern cy_error_t cy_mem_check(cy_mem_ctx_t *io_mem);
extern cy_error_t cy_mem_defrag(cy_mem_ctx_t *io_mem);
extern cy_error_t cy_mem_uninit(cy_mem_ctx_t *io_mem);

#define _MEM_ERASED_READY 0xCA /* value chosen because harder to fault attack*/
#define _MEM_ERASED_LOCKED 0x59 /* value chosen because harder to fault attack*/

#define _MEM_ALLOCATED                                                 \
  0xA5 /* value chosen because harder to fault attack*/

#endif
