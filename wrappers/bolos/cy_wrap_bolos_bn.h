/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos_bn.h										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Multiprecision Integers API                                       */
/**********************************************************************************/


#ifndef API_CY_WRAP_BOLOS_BN_H_
#define API_CY_WRAP_BOLOS_BN_H_

#define cy_bn_shift_r(shift, out) cy_wrap_bolos_bn_shift_r(shift,out)

#define cy_bn_or(a,b,out) cy_wrap_bolos_bn_or(a,b,out)


#endif
