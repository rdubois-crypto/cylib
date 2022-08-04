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
/* DESCRIPTION: Elliptic curves  CY_LIB native implementation */
/**********************************************************************************/


#ifndef CY_DEF_H_
#define CY_DEF_H_

/* the max bytesize of a name (library, object, etc.)*/
#define MAX_SIZENAME 32

typedef  enum {
    MSB_STRING = 1, /* the most popular representation: data is ordered from MSB to LSB, ie 257 is {0x1, 0xff}*/
    LSB_STRING = 2, /* mathematical friendly representation: data is ordered from LSB to MSB, ie 257 is {0xff, 0x1}*/
    PKCS = 3,
    SHUFFLED = 4,
	COMPRESSED_MSB =5, /* compressed point, last byte is parity of y value*/
	UNCOMPRESSED_MSB =6 /* uncompressed point */
} DATA_FORMAT;



#endif /* SRC_INNOVATION_CY_DEF_H_ */
