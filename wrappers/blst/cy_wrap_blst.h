/*
 * cy_wrap_blst.h
 *
 *  Created on: Jul 5, 2022
 *      Author: dubois
 */

#ifndef SRC_INNOVATION_CY_WRAP_BLST_H_
#define SRC_INNOVATION_CY_WRAP_BLST_H_

struct fp_ctx_s{
  limb_t *modular;
  size_t t8_modular;
  char libname[]="blst";

};

typedef struct fp_ctx_s fp_ctx_t;

/* Wrapping blst functions to cylib*/
#define cy_fp_add(c,a,b,r) wrap_blst_fp_add(c,a,b,r)
#define cy_fp_sub(c,a,b,r) wrap_blst_fp_add(c,a,b,r)
#define cy_fp_mult_mont(c,a,b,r) wrap_blst_fp_mult_mont(c,a,b,r)


#endif /* SRC_INNOVATION_CY_WRAP_BLST_H_ */
