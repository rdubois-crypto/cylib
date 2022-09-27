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
/* DESCRIPTION: error handling API */
/**********************************************************************************/
#ifndef _CY_ERRORS_H
#define _CY_ERRORS_H

#define UNUSED(x) (void)(x)



#ifdef _NO_CONTROL
typedef void cy_error_t
#else
typedef int cy_error_t;
typedef int cy_flag_t;
#endif

#define CY_LIB_INITIALIZED ((int)0x11111111)
#define CY_LIB_ALLOCATED   ((int)0x22222222)
#define CY_LIB_UNINITIALIZED ((int)0x44444444)

#define CY_INCOMPLETE_IMPLEMENTATION 0xfffffffffffffffa

#define CY_NOT_IMPLEMENTED 0x506

/* Boolean code*/
#define CY_OK 0
#define CY_KO 1

#define CY_TRUE  1
#define CY_FALSE 0

#define CY_EQU 0

#define CY_ERR_ALLOC ((int)0xfffffff1)
#define CY_ERR_INIT  ((int)0xffffff3)

#define CY_ERR_UNIT   ((int)0xffffffa)
#define CY_ERR_LENGTH ((int)0xffffffb)


/* Memory unit errors */
#define CY_MEM_OVERFLOW 0xfffffff1
#define CY_MEM_FREE_INDEX 0xfffffff2
#define CY_MEM_FREE_MISSING 0xf3fffff2

/* Modular integers errors*/

/* Elliptic curve  errors*/

#define CY_UNHANDLED_CURVE 0xfffff33

/* Quadratic extensions  errors*/

/* Pairing computations errors*/


/* MACROS DEFINITIONS */
#define CY_UNUSED(x) x

/* This is the controlled version of error_checking macros, inactivate for faster (but less secure ) version*/
#ifndef _NO_CONTROL
/* declare an error in function beginning */
//#define DECLARE(type, c) {##a  ##c}

#define CY_ERROR_INIT()	cy_error_t error


/* label for the CY_CHECK goto */
#define CY_ERROR_CLOSE() do{\
	end:\
		return error;\
}while(0)

#define CY_ERROR_RETURN(_ERR) do{\
	error=_ERR;\
	goto end;\
}while(0);

/* test return function and go to label end if not ok*/
#define CY_CHECK(call)                                                         \
  do {                                                                         \
    error = call;                                                              \
    if (error) {                                                               \
      goto end;                                                                \
    }                                                                          \
  } while (0)

#define CY_IS_INIT(obj)                                                         \
  do {                                                                         \
                                                               \
    if (obj->is_initialized != CY_LIB_INITIALIZED) {                             \
    	  error = CY_ERR_INIT;                                                              \
      goto end;                                                                \
    }                                                                          \
  } while (0)
#else
/* TODO: the no control implementation */
#endif

#define CY_UNIMPLEMENTED_F(function_name, args...)\
	void ##function_name(args){\
	  return CY_NOT_IMPLEMENTED;\
  }

#endif
