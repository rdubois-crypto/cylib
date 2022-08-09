/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_lib.h*/
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Bolos Library initialization and management API */
/**********************************************************************************/

#ifndef CY_WRAP_SEC256K1_H_
#define CY_WRAP_SEC256K1_H_



cy_mem_ctx_t lib256k1_mem_unit;

#define cy_lib_init(cryptolib, Shared_Memory, allocated_t8, additional,        \
                    additional_t8)                                             \
  cy_wrap_lib256k1_init(cryptolib, Shared_Memory, allocated_t8, additional,       \
                     additional_t8)
#define cy_lib_check(i_cryptolib)  cy_wrap_lib256k1_check(i_cryptolib)
#define cy_lib_uninit(i_cryptolib) cy_wrap_lib256k1_uninit(i_cryptolib)




#endif /* SRC_INNOVATION_CY_WRAP_SEC256K1_H_ */
