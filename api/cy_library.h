/**********************************************************************************/
/* Copyright (C) 2017 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_word64.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: API for library wrappers, handling of external libraries		  */
/**********************************************************************************/
#ifndef API_CY_LIBRARY_H_

#include <stdint.h>
#include "cy_errors.h"

#define _SIZE_NAME_LIBRARY 12

struct cy_libctx_s{
 /* name of the library, ex: libECC_1.0*/
 unsigned char Identifier[_SIZE_NAME_LIBRARY];
 /* pointer to allocated key memory */
 unsigned char *Key_mem;
 /* pointer to allocated shared ram*/
 unsigned char *Ramp;
 int is_initialized=CY_FALSE;
};


typedef cy_libctx_s cy_libctx_t;

cy_error_t cy_init_library(cy_libctx_t *io_cryptolib, const uint8_t *i_initializer);

cy_error_t cy_close_library(cy_libctx_t *io_cryptolib, const uint8_t *i_initializer);


#define API_CY_LIBRARY_H_





#endif /* API_CY_LIBRARY_H_ */
