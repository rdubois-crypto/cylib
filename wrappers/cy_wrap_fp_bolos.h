/*
 * cy_wrap_bolos.h
 *
 *  Created on: Jul 5, 2022
 *      Author: dubois
 */

#ifndef SRC_INNOVATION_CY_WRAP_FP_BOLOS_H_
#define SRC_INNOVATION_CY_WRAP_FP_BOLOS_H_

#define BOLOS_LIBNAME "lib_bolos_vX"
#define _MAX_MEMORY 256 /* max size of shared memory*/

#define _MAPPING_START (0)
#define _CX_BN_P (1)

#define cy_fp_init(a,b,c,d,e)		(wrap_bolos_fp_init(a,b,c,d,e))

#define cy_fp_alloc(c,s,r) (wrap_bolos_fp_alloc(c,s,r))

#define cy_fp_free(fp) (wrap_bolos_fp_free(fp))

#define cy_fp_import(c,in,size,out) (wrap_bolos_fp_import(c,in,size,out))


#define cy_fp_add(c,a,b,r) (wrap_bolos_fp_add(c,a,b,r))


#define cy_fp_uninit(ctx,mem, size) (wrap_bolos_fp_uninit(ctx,mem,size))



#define cy_fp_t cx_bn_t

#endif /* SRC_INNOVATION_CY_WRAP_FP_BOLOS_H_ */
