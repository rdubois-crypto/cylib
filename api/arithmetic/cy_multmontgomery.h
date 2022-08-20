/**********************************************************************************/
/* FILE: cy_mulmont_core.h
 */
/* A self sufficient implementation of a 128 bits Montgomery multiplication */
/* 																			      */
/* The code is intended to enable 32, 64 and 128 bits multiplier in later
 * version */
/**********************************************************************************/

#include "stddef.h"

void MontGomery_Mult(cy_word_t r[], const cy_word_t a[], const cy_word_t b[],
                     const cy_word_t p[], const cy_word_t n0, size_t size);
