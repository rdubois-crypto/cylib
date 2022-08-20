/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_word64.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Interoperability word header
 */
/**********************************************************************************/
#ifndef _WORD64_H
#define _WORD64_H

#include <stdint.h>

#define WORD64_BITS 64
#define WORD64_MASK 0xffffffffffffffff

//#define __x86_64__
#ifdef __x86_64__
static inline void mul64x64to128(uint64_t *low, uint64_t *high, uint64_t x,
                                 uint64_t y)
{
  __asm__("mulq %3" : "=a"(*low), "=d"(*high) : "%0"(x), "rm"(y) : "cc");
}

#define WORD64_MUL(out, in1, in2) mul64x64to128(out, out + 1, in1, in2)

#else
#define WORD64_MUL(out, in1, in2)                                              \
  do {                                                                         \
    uint64_t in1h, in1l, in2h, in2l;                                           \
    uint64_t tmph, tmpm, tmpl;                                                 \
    uint64_t tmpm1, tmpm2;                                                     \
    uint64_t carrym, carryl;                                                   \
    /* Get high and low half words. */                                         \
    in1h = (in1) >> WORD32_BITS;                                               \
    in1l = (in1)&WORD32_MASK;                                                  \
    in2h = (in2) >> WORD32_BITS;                                               \
    in2l = (in2)&WORD32_MASK;                                                  \
    /* Compute low product. */                                                 \
    tmpl = (uint64_t)(in2l * in1l);                                            \
    /* Compute middle product. */                                              \
    tmpm1 = (uint64_t)(in2h * in1l);                                           \
    tmpm2 = (uint64_t)(in2l * in1h);                                           \
    tmpm = (uint64_t)(tmpm1 + tmpm2);                                          \
    /* Store middle product carry. */                                          \
    carrym = (uint64_t)(tmpm < tmpm1);                                         \
    /* Compute full low product. */                                            \
    (out[0]) = tmpl;                                                           \
    (out[0]) = (uint64_t)((out[0]) + ((tmpm & WORD32_MASK) << WORD32_BITS));   \
    /* Store full low product carry. */                                        \
    carryl = (uint64_t)((out[0]) < tmpl);                                      \
    /* Compute full high product. */                                           \
    carryl = (uint64_t)(carryl + (tmpm >> WORD32_BITS));                       \
    carryl = (uint64_t)(carryl + (carrym << WORD32_BITS));                     \
    tmph = (uint64_t)(in2h * in1h);                                            \
    /* No carry can occur below. */                                            \
    (out[1]) = (uint64_t)(tmph + carryl);                                      \
  } while (0)
#endif

#endif
