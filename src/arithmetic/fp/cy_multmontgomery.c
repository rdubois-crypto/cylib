/****************************************************************************************/
/* FILE: cy_mulmont_core.c
 */
/* A self sufficient implementation of a 128 bits Montgomery multiplication
 */
/* 																			      		*/
/* The code is intended to enable 32, 64 and 128 bits multiplier in later
 * versions		*/
/* A description of the algorithm is available at
 */
/* https://www.microsoft.com/en-us/research/wp-content/uploads/1996/01/j37acmon.pdf
 */
/****************************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "cy_errors.h"
#include "cy_io.h"
#include "cy_multmontgomery.h"
#include "cy_word.h"

#define WORD32_BITS 32
#define WORD32_MASK 0xffffffff

typedef int cx_err_t;
/* Forked from libECC for emulated words
 * Compare given two nn 'A' and '. This is done *in constant time* (only
 * depending on the largest length of the inputs, not on their values). The
 * function returns 0 on success and provides the comparison value in
 * 'cmp' parameter (0 if A == B, -1 if A < B, +1 if A > B). -1 is returned
 * on error, in which case 'cmp' is not meaningful.
 */
void cy_mpz_cmp(cy_word_t a[], cy_word_t b[], int size, int *cmp)
{
  int tmp, mask, carry, i;

  tmp = CY_EQU;
  for (i = (size - 1); i >= 0; i--) { /* ok even if cmp_len is 0 */
    mask = !(tmp & 0x1);
    INFEQ_MWORD(b[i], a[i], carry);
    tmp += (carry & mask);
    INF_MWORD(a[i], b[i], carry);
    tmp -= (carry & mask);
  }
  (*cmp) = tmp;
} /* LEVEL_TEST=0*/

/* Conditional substraction, forked from nn_cnd_sub of libECC for generic size
 * (including vectors)*/
int cy_mpz_sub(int cnd, cy_word_t *out, cy_word_t *a, cy_word_t *b, size_t size)
{
  cy_word_t tmp, borrow1, borrow2, borrow;
  ZERO_MWORD(borrow);
  word_t mask = WORD_MASK_IFNOTZERO(cnd);
  size_t loop_wlen, i;
  int ret;

  /* Perform subtraction one word at a time, propagating the borrow. */
  for (i = 0; i < loop_wlen; i++) {
    // tmp = (word_t)(in1->val[i] - (in2->val[i] & mask));

    SUB_MWORD(tmp, a[i], b[i], borrow1);

    //		borrow1 = (word_t)(tmp > in1->val[i]);
    SUBMWORD(out[i], tmp, borrow, borrow2);
    //		borrow2 = (word_t)(out->val[i] > tmp);
    /* There is at most one borrow going out. */
    borrow = (word_t)(borrow1 | borrow2);
  }

  /* We only support the in1 >= in2 case */
  ret = -ISZERO_MWORD(borrow);

  return ret;
} /* LEVEL_TEST=0*/

/* C implementation:  custom CIOS implementation  better than FIOS  */
/* TODO: make implementation fully interoperable, there are some implicit low
 * and high part calls*/
void cy_mpmod_montgmult(cy_word_t r[], const cy_word_t a[], const cy_word_t b[],
                        const cy_word_t p[], const cy_word_t n0, size_t size)
{
  register cy_word_t l_carry, u, rb, temp;
  static double_montgomery_t tn;
  uint64_t tu64_carry = 0;

  size_t i, j;
  ZERO_MWORD(temp);

  printf("\n # size=%d", size);
  for (i = 0; i <= size; i++)
    ZERO_MWORD(r[i]); // r[i]=0;

  for (i = 0; i < size; i++) {
    tu64_carry = 0;
    ZERO_MWORD(l_carry);
    printf("\n # *********************** i=%d", i);
    /*R=R+a*b[i]*/
    for (j = 0; j < size; j++) {
      /*(tn[0],tn[1])=r[j]+a[j]*b[i]+carry*/
      MUL_MWORD(tn, b[i], a[j]); //  SPInt_Mul(b[i],a[j],tn);
      IO_print_montgo("\n# Preadd tn0:", tn[0]);
      IO_print_montgo("\n# tn1:", tn[1]);

      ADDTO_DWORD(tn, tu64_carry, l_carry); //  SPInt_Acc(tn,carry);

      ADDTO_DWORD(tn, tu64_carry, r[j]); // SPInt_Acc(tn,r[j]);
      printf("\n # i=%d j=%d", i, j);
      IO_print_montgo("\n# tn0:", tn[0]);
      IO_print_montgo("\n# tn1:", tn[1]);

      COPY_MWORD(r[j], tn[0]);    // r[j]=tn[0];
      COPY_MWORD(l_carry, tn[1]); // carry=tn[1];
    }
    ADDCTO_MWORD(r[size], tu64_carry,
                 l_carry); // r[size]+=carry;//devil carry here
    IO_print_montgo_array("\n# r loop1: ", r, 3);

    temp[0] = tu64_carry; // temp=(r[size]<carry?1:0);//propagated by addtom in
                          // tu64_carry
    printf("\n # temp=%d", temp[1]);

    WORD_MUL_LOW(u, r[0], n0); // u=r[0]*pModData->NPrime;

    MUL_MWORD(tn, u, p[0]);            // SPInt_Mul(u,pModData->Modulus[1],tn);
    ADDTO_DWORD(tn, tu64_carry, r[0]); // SPInt_Acc(tn,r[0]);
    COPY_MWORD(l_carry, tn[1]);        // carry=tn[1];

    /*R=R+u[i]*Modulus*/
    for (j = 1; j < size; j++) {
      printf("\n # i=%d j=%d", i, j);
      /*(tn[0],tn[1])=r[j]+u*Modulus[j+1]+carry */
      MUL_MWORD(tn, u, p[j]); // SPInt_Mul(pModData->Modulus[j+1],u,tn);

      ADDTO_DWORD(tn, tu64_carry, r[j]); // SPInt_Acc(tn,r[j]);

      ADDTO_DWORD(tn, tu64_carry, l_carry); // SPInt_Acc(tn,carry);

      IO_print_montgo("\n# tn0:", tn[0]);
      IO_print_montgo("\n# tn1:", tn[1]);

      /*Allocating and shifting simultaneously*/
      COPY_MWORD(r[j - 1], tn[0]); // r[j-1]=tn[0];
      COPY_MWORD(l_carry, tn[1]);  // carry=tn[1];
    }
    COPY_MWORD(tn[0], r[size]);           //        tn[0]=r[size];
    ZERO_MWORD(tn[1]);                    // tn[1]=(SPInt_t)0;
    ADDTO_DWORD(tn, tu64_carry, l_carry); // SPInt_Acc(tn,carry);

    COPY_MWORD((r)[size - 1], tn[0]); //(r)[size-1]=tn[0];
    IO_print_montgo_array("\n# r loop2: ", r, 3);

    ZERO_MWORD((r)[size]);
    ADDTO_MWORD((r)[size], tu64_carry, temp); //(r)[size]=temp+tn[1];
    ADDTO_MWORD((r)[size], tu64_carry, tn[1]);
    IO_print_montgo("\n# r[size]:", r[size]);
  }

  if (r[size][0]) {
    printf("\n # sub cause & extra bit");
    goto sub;
  }

  for (i = size; i; i--) {
    if (p[i][1] > r[i][1])
      return;
    else if (p[i][1] < r[i][1])
      goto sub;
    if (p[i][0] > r[i][0])
      return;
    else if (p[i][0] < r[i][0])
      goto sub;
  }

  if (p[0][1] > r[0][1])
    return;
  else if (p[i][1] < r[i][1])
    goto sub;

  if (p[0][0] > r[0][0])
    return;
/*
 * Note that at this stage the msw of out is either 0 or 1.
 * If out > p we need to subtract p from out.
 * It is performed isochronously constant time as in libECC
 */
sub:

  printf("\n SOUSTRACTION !!!!!!!!!!!!!!!!!!");
  /* substraction r=A-N */

  tu64_carry = 0;
  for (i = 0; i <= size; i++) {
    SUB_MWORD(rb, r[i], p[i], tu64_carry);
    if (!tu64_carry) {
      INF_MWORD(r[i], p[i], tu64_carry);
    } else {
      INFEQ_MWORD(r[i], p[i], tu64_carry);
    }
    COPY_MWORD(r[i], rb);
  }

  return;
} /* LEVEL_TEST=1*/
