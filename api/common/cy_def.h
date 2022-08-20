/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_def.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION:   CY_LIB common definitions */
/**********************************************************************************/


#ifndef CY_DEF_H_
#define CY_DEF_H_

/* the max bytesize of a name (library, object, etc.)*/
#define MAX_SIZENAME 32

#define boolean_t int

typedef  enum {
    MSB_STRING = 1, /* the most popular representation: data is ordered from MSB to LSB, ie 257 is {0x1, 0xff}*/
    LSB_STRING = 2, /* mathematical friendly representation: data is ordered from LSB to MSB, ie 257 is {0xff, 0x1}*/
    PKCS = 3,
    SHUFFLED = 4,
	COMPRESSED_MSB =5, /* compressed point, last byte is parity of y value*/
	UNCOMPRESSED_MSB =6 /* uncompressed point */
} DATA_FORMAT;

/* this stipulates that the object is a CY_LIB native function */
#define _CY_NATIVE

/* this stipulates that the object is a CY_LIB wrapping function */
#define _CY_WRAP

/* this stipulates that the object is a CY_LIB API  */
#define _CY_API

/* headers definition */


#define word32_from_be(a) (a[0] + (a[1] << 8) + (a[2] << 16) + (a[3] << 24))


#endif /* SRC_INNOVATION_CY_DEF_H_ */
