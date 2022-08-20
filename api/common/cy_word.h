/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project */
/* License: This software is licensed under a dual BSD and GPL v2 license */
/* See LICENSE file at the root folder of the project. */
/*                                                                                */
/* 																			      */
/* FILE: cy_word128.h */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Interoperability word header */
/**********************************************************************************/

#ifndef _CY_WORD_H
#include "cy_word64.h"
/* Configuration of the size of the Montgomery Multiplier*/
#define _SIZE_WORD 128

/* not tested*/
#if _SIZE_WORD == 64
#include "cy_word64.h"
#endif

/*under development*/
#if _SIZE_WORD == 128

#include "cy_word128.h"
#define ZERO_MWORD(a)             ZERO_MWORD128(a)
#define COPY_MWORD(dest, source)  COPY_MWORD128(dest, source)
#define ADDCTO_MWORD(res, c, in)  ADDCTO_MWORD128(res, c, in)
#define ADDTO_MWORD(res, c, in)   ADDTO_MWORD128(res, c, in)
#define MUL_MWORD(a, c, d)        WORD128_MUL(a[0], a[1], c, d)
#define INF_MWORD(a, b, carry)    INF_MWORD128(a, b, carry)
#define INFEQ_MWORD(a, b, carry)  INFEQ_MWORD128(a, b, carry)
#define SUB_MWORD(r, a, b, carry) SUB_MWORD128(r, a, b, carry)
#define ADDTO_DWORD(res, c, in)   ADDTO_DWORD128(res, c, in)
#define WORD_ADDC                                                              \
  WORD128_ADDC(res, carry, in1, in2)                                           \
  WORD128_ADDC WORD128_ADDC(res, carry, in1, in2)
#define WORD128_ADD(res, carry, in1, in2) WORD_ADD(res, carry, in1, in2)
#define WORD_MUL_LOW(outl, in1, in2)      WORD128_MUL_LOW(outl, in1, in2)
#define ISZERO_MWORD(m)                   ((m[0] == 0) & (m[1] == 0))
#define SET_LOW(m, a)                     (m[0] = a)
typedef uint128_t word_montgomery_t;
typedef word_montgomery_t double_montgomery_t[2];
/* the words are stored [1]:msb, [0]:lsb. Throw a coin. If you loose curse me.*/
typedef uint128_t cy_word_t;

#endif

#endif
