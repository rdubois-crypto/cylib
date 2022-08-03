/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_word64.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: error handling API		                                              */
/**********************************************************************************/
#ifndef _CY_ERRORS_H
#define _CY_ERRORS_H

#define UNUSED(x) (void)(x)

#define CY_CHECK(call)                                                         \
  do {                                                                         \
    error = call;                                                              \
    if (error) {                                                               \
      goto end;                                                                \
    }                                                                          \
  } while (0)

typedef int cy_error_t;
typedef int cy_flag_t;

#define CY_LIB_INITIALIZED ((int) 0xfffffffb)
#define CY_LIB_UNITIALIZED ((int) 0xffffffcb)

#define CY_INCOMPLETE_IMPLEMENTATION 0xfffffffffffffffa

/* Boolean code*/
#define CY_OK 0
#define CY_KO 1

#define CY_TRUE 1
#define CY_FALSE 0

#define CY_EQU 0

#define CY_ERR_ALLOC ((int) 0xfffffff1)
#define CY_ERR_INIT  ((int) 0xffffff3)

#define CY_ERR_UNIT  ((int) 0xffffffa)
#define CY_ERR_LENGTH ((int) 0xffffffb)


/* Memory unit errors */
#define CY_MEM_OVERFLOW 0xfffffffffffff1

#define CY_MEM_FREE_INDEX 0xfffffffffffff2

/* Modular integers errors*/

/* Elliptic curve  errors*/

#define CY_UNHANDLED_CURVE 0xffffffffffffff33

/* Quadratic extensions  errors*/

/* Pairing computations errors*/


#define CY_UNUSED(x) x

#endif
