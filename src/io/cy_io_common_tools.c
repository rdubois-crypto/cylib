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



/*activate this define for debug information*/
//#define _DEBUG_BURRITOS


#include <stdint.h>
#include <stddef.h>
#include "cy_io_common_tools.h"

#if (defined(_DEBUG_BURRITOS)) || (!defined(_TEST_ALL))
#include <stdio.h>

/* variadic  ISO/IEC 9899:1999*/
#define debug_printf(...) printf ( __VA_ARGS__)
//#define debug_printf(...) UNUSED(0)

 void debug_Print_RAMp(uint8_t *Ramp, size_t RAM_t8)
{
  size_t i;
  debug_printf("\n RAM:\n");
  for (i = 0; i < RAM_t8; i++) {
	  debug_printf("%02X", Ramp[i]);
    if ((i & 0xf) == 15)
    	debug_printf("\n");
  }
}


#endif


 void print_MsbString(uint8_t *String, size_t String_t8, char *prefix)
{
 size_t i;
 printf("%s\n ", prefix);
 if(String_t8==0) {
	 printf("{}");
	 return;
 }
printf("{");
 printf(" 0x%02X",String[0]);

 for(i=1;i<String_t8;i++)
 {
	 printf(", 0x%02X",String[i]);
	 if ((i & 0xf) == 15)
	     	printf("\n ");
 }
 printf("  }");

}

 void print_LsbString(uint8_t *String, size_t String_t8, char *prefix)
{
 int i=(int) String_t8;
 printf("%s\n ", prefix);
 if(String_t8==0) {
	 printf("{}");
	 return;
 }
printf("{");
 printf(" 0x%02X",String[String_t8-1]);

 for(i=String_t8-2;i>0;i--)
 {
	 printf(", 0x%02X",String[i]);
	 if ((i & 0xf) == 15)
	     	printf("\n ");
 }
 printf("  }");

}
