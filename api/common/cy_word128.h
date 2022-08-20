/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project */
/* License: This software is licensed under a dual BSD and GPL v2 license. */
/* See LICENSE file at the root folder of the project. */
/* 																			      */
/* FILE: cy_word128.h */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Interoperability word header */
/**********************************************************************************/
#ifndef _WORD128_H
#define _WORD128_H

#include "cy_word64.h"

typedef uint64_t uint128_t[2];

#define ZERO_MWORD128(a)                                                       \
  do {                                                                         \
    a[0] = 0;                                                                  \
    a[1] = 0;                                                                  \
  } while (0)

#define COPY_MWORD128(dest, source)                                            \
  do {                                                                         \
    dest[0] = source[0];                                                       \
    dest[1] = source[1];                                                       \
  } while (0)

#define ADDCTO_MWORD128(res, c, in)                                            \
  do {                                                                         \
    res[0] += in[0];                                                           \
    c = res[0] < in[0];                                                        \
    res[1] += in[1] + c;                                                       \
    c = res[1] < in[1];                                                        \
  } while (0)

#define ADDTO_MWORD128(res, c, in)                                             \
  do {                                                                         \
    res[0] += in[0];                                                           \
    c = res[0] < in[0];                                                        \
    res[1] += in[1] + c;                                                       \
  } while (0)

#define INF_MWORD128(a, b, carry)                                              \
  do {                                                                         \
    if (a[1] < b[1])                                                           \
      carry = 1;                                                               \
    if (a[0] < b[0])                                                           \
      carry = 1;                                                               \
    carry = 0;                                                                 \
  } while (0)

#define INFEQ_MWORD128(a, b, carry)                                            \
  do {                                                                         \
    if (a[1] < b[1])                                                           \
      carry = 1;                                                               \
    if (a[0] <= b[0])                                                          \
      carry = 1;                                                               \
    carry = 0;                                                                 \
  } while (0)

#define SUB_MWORD128(r, a, b, carry)                                           \
  do {                                                                         \
    r[0] = a[0] - b[0] - carry;                                                \
    carry = (a[0] < b[0] + carry);                                             \
    r[1] = a[1] - b[1] - carry;                                                \
    carry = (a[1] < b[1] + carry);                                             \
  } while (0)

#define ADDTO_DWORD128(res, c, in)                                             \
  do {                                                                         \
    res[0][0] += in[0];                                                        \
    c = (res[0][0] < in[0]);                                                   \
    res[0][1] += c;                                                            \
    c = (res[0][1] < c);                                                       \
    res[0][1] += in[1];                                                        \
    c |= (res[0][1] < in[1]);                                                  \
    res[1][0] += c;                                                            \
    res[1][1] += (c > res[1][0]);                                              \
  } while (0)

/* WORD_MUL: multiply two words using schoolbook multiplication on half words */
/* from: libecc::words.h, using custom modifications*/
#define WORD128_MUL(outl, outh, in1, in2)                                      \
  do {                                                                         \
    uint128_t l_tmph, l_tmpm, l_tmpl;                                          \
    uint128_t l_tmpm1, l_tmpm2;                                                \
    uint128_t l_carryl;                                                        \
    uint64_t l_carrym;                                                         \
    /* Compute low product. */                                                 \
    WORD64_MUL(l_tmpl, in2[0], in1[0]);                                        \
    /* Compute middle product with its l_carry. */                             \
    WORD64_MUL(l_tmpm1, in2[1], in1[0]);                                       \
    WORD64_MUL(l_tmpm2, in2[0], in1[1]);                                       \
    WORD128_ADDC(l_tmpm, l_carrym, l_tmpm1, l_tmpm2);                          \
    /* Compute full low product. */                                            \
    (outl[0]) = l_tmpl[0];                                                     \
    (outl[1]) = l_tmpl[1] + l_tmpm[0];                                         \
    /* Store full low product l_carry. */                                      \
    l_carryl[0] = ((outl[1]) < l_tmpl[1]);                                     \
    /* Compute full high product. */                                           \
    l_carryl[0] = (l_carryl[0] + (l_tmpm[1]));                                 \
    l_carryl[1] = (l_carryl[0] < (l_tmpm[1]));                                 \
    l_carryl[1] += (l_carrym);                                                 \
    WORD64_MUL(l_tmph, in2[1], in1[1]);                                        \
    /* No l_carry can occur below. */                                          \
    WORD128_ADDC(outh, l_carrym, l_tmph, l_carryl);                            \
  } while (0)

#define WORD128_MUL_LOW(outl, in1, in2)                                        \
  do {                                                                         \
    uint128_t l_tmph, l_tmpm, l_tmpl;                                          \
    uint128_t l_tmpm1, l_tmpm2;                                                \
    uint128_t l_carryl;                                                        \
    uint64_t l_carrym;                                                         \
    /* Compute low product. */                                                 \
    WORD64_MUL(l_tmpl, in2[0], in1[0]);                                        \
    /* Compute middle product with its l_carry. */                             \
    WORD64_MUL(l_tmpm1, in2[1], in1[0]);                                       \
    WORD64_MUL(l_tmpm2, in2[0], in1[1]);                                       \
    WORD128_ADDC(l_tmpm, l_carrym, l_tmpm1, l_tmpm2);                          \
    /* Compute full low product. */                                            \
    (outl[0]) = l_tmpl[0];                                                     \
    (outl[1]) = l_tmpl[1] + l_tmpm[0];                                         \
  } while (0)

/* Add two 128 bits words with carry. Custom. Cannot be used 'in place' */
/* Hypothesis : no overflow of max value occurs here*/
#define WORD128_ADDC(res, carry, in1, in2)                                     \
  do {                                                                         \
    res[0] = in1[0] + in2[0];                                                  \
    carry = res[0] < in1[0];                                                   \
    res[1] = in1[1] + in2[1] + carry;                                          \
    carry = res[1] < in1[1];                                                   \
  } while (0)

#define WORD128_ADD(res, carry, in1, in2)                                      \
  do {                                                                         \
    res[0] = in1[0] + in2[0];                                                  \
    carry = res[0] < in1[0];                                                   \
    res[1] = in1[1] + in2[1] + carry;                                          \
    carry = res[1] < in1[1];                                                   \
  } while (0)

#endif
