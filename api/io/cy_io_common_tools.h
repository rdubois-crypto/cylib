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


/*activate this define for debug information*/
//#define _DEBUG_BURRITOS

extern  void print_MsbString(uint8_t *String, size_t String_t8, char *prefix);

#if (defined(_DEBUG_BURRITOS)) || (!defined(_TEST_ALL))

#define _debug(cmd) cmd

extern  void debug_Print_RAMp(uint8_t *Ramp, size_t RAM_t8);


#else
#define debug_printf(...) UNUSED(0)
#define debug_Print_RAMp(a,s) UNUSED(0)
#define debut_print_MsbString(a,s,comment) UNUSED(0)

#define _debug(cmd) UNUSED(0)

#endif /* Debug*/

#endif /*_TEST_COMMON_TOOLS_H_*/
