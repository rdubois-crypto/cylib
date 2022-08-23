/*
 * cy_pedersencommit.h
 *
 *  Created on: Aug 23, 2022
 *      Author: dubois
 */

#ifndef CY_PEDERSENCOMMIT_H_
#define CY_PEDERSENCOMMIT_H_

#include "cy_errors.h"

cy_error_t cy_pedersen_init();
cy_error_t cy_pedersen_commit();
cy_error_t cy_pedersen_open();
cy_error_t cy_pedersen_uninit();


#endif /* SRC_INNOVATION_API_PROTOCOLS_CY_PEDERSENCOMMIT_H_ */
