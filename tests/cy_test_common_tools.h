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




#if (defined(_DEBUG_BURRITOS)) || (!defined(_TEST_ALL))

/* variadic  ISO/IEC 9899:1999*/
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


static void debut_print_MsbString(uint8_t *String, size_t String_t8, char *prefix)
{
 size_t i;
 debug_printf("%s", prefix);
 if(String_t8==0) {
	 debug_printf("{}");
	 return;
 }
 debug_printf("{");
 debug_printf(" 0x%02X",String[0]);

 for(i=1;i<String_t8;i++)
 {
	 debug_printf(", 0x%02X",String[i]);
	 if ((i & 0xf) == 15)
	     	debug_printf("\n");
 }
 debug_printf("}");

}

#else
#define debug_printf(...) UNUSED(0)
#define Print_RAMp(a,s) UNUSED(0)
#define debut_print_MsbString(a,s,comment) UNUSED(0)
#endif

#endif
