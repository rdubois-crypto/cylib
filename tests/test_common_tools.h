/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois                                             */
/* This file is part of cy_lib project                                            */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_common_tools.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing all modules and lib                       */
/**********************************************************************************/


#ifndef _TEST_COMMON_TOOLS_H_
#define _TEST_COMMON_TOOLS_H_


#if !defined(_DEBUG_BURRITOS) && !defined(_TEST_ALL)


#define debug_printf(...) printf ( __VA_ARGS__)
//#define debug_printf(...) UNUSED(0)

static void Print_RAMp(uint8_t *Ramp, size_t RAM_t8)
{
  size_t i;
  debug_printf("\n RAM:\n");
  for (i = 0; i < RAM_t8; i++) {
	  debug_printf("%02X", Ramp[i]);
    if ((i & 0xf) == 15)
    	debug_printf("\n");
  }
}

#else
#define debug_printf(...) UNUSED(0)
#define Print_RAMp(a,s) UNUSED(0)
#endif

#endif
