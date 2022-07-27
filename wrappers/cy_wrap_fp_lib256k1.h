

#ifndef SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_
#define SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_

#define LIB256K1_LIBNAME "libsecp256k1"


#include "libsec256k1_field_10x26.h"

#define _MAX_MEMORY 256 /* max size of shared memory*/


#define cy_fp_t secp256k1_fe

#define cy_fp_init(a,b,c,d,e)		(wrap_lib256k1_fp_init(a,b,c,d,e))



#endif /* SRC_INNOVATION_CY_WRAP_FP_LIB256K1_H_ */
