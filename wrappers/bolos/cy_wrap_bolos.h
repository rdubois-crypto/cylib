/*
 * cy_wrap_bolos.h
 *
 *  Created on: Jul 5, 2022
 *      Author: dubois
 */

#ifndef SRC_INNOVATION_CY_WRAP_BOLOS_H_
#define SRC_INNOVATION_CY_WRAP_BOLOS_H_

#define T8_MAX_LIBNAME 12
#define BOLOS_LIBNAME "lib_bolos_vX"
#define _MAX_MEMORY 256 /* max size of shared memory*/

#define _MAPPING_START (0)
#define _CX_BN_P (1)

#define cy_fp_init(a,b,c,d,e) wrap_bolos_fp_init(a,b,c,d,e)

#define cy_fp_add(c,a,b,r) wrap_blst_fp_add(c,a,b,r)

#define cy_fp_t cx_bn_t

#endif /* SRC_INNOVATION_CY_WRAP_BOLOS_H_ */
