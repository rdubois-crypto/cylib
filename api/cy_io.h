/**********************************************************************************/
/* Copyright (C) 2017 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_word64.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: API for IO handling		                                          */
/**********************************************************************************/

#ifndef _CY_IO_H
#define _CY_IO_H

#include "cy_multmontgomery.h"


 void IO_print_montgo(char *comment, word_montgomery_t w);
 void IO_print_montgo_array(char *comment, word_montgomery_t *w, size_t size);

 #define debug_printf(a,x) do{printf(a,x);}while(0)
#endif

