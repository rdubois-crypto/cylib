/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois                                             */
/* This file is part of cy_lib project                                            */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_cy_mem_unit.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing all modules and lib                       */
/**********************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "innovation/cy_def.h"
#include "innovation/cy_errors.h"
#include "innovation/cy_memory_unit.h"
#include "innovation/cy_gda_component.h"


#define _TEST_ALL /* enable testing of all modules */

/*Pick a burritos to be tested */
#define _BOLOS_BURRITOS /* bolos*/
//#define _LIB256K1_BURRITOS

#if defined(_BOLOS_BURRITOS)
#include "innovation/cy_wrap_bolos_gda.h"
#include "innovation/cy_wrap_bolos_cryptolib.h"
#endif

#include "test_cy_mem_unit.c"
#include "test_wrap_fp.c"


cy_error_t test_all(cryptolib_ctx_t *cryptolib)
{
	cy_error_t error = CY_OK;

	CY_CHECK(test_mem_unit(cryptolib));
	CY_CHECK(test_fp_unit(cryptolib->mem_unit->Shared_Memory, cryptolib->mem_unit->allocated_t8));

	printf("\n here error=%d", error);
	  end:
	  return error;
}

int main()
{
	cy_error_t error = CY_OK;
	uint8_t Ramp[_MAX_MEMORY];
	cryptolib_ctx_t cryptolib;

	  printf("\n/************************ Test All *****************************************/");

	  debug_printf("\n @RAMP=%x", (unsigned int)Ramp);
	  Print_RAMp(Ramp, _MAX_MEMORY);

	  printf("\n Starting lib, @RAMP=%x", (unsigned int)Ramp);
	  CY_CHECK(cy_lib_init(&cryptolib, Ramp, sizeof(Ramp), NULL, 0));
	  Print_RAMp(Ramp, _MAX_MEMORY);

	  CY_CHECK(test_all(&cryptolib));

	  printf("\n\n /************************ Test Closing lib:", (unsigned int)Ramp);
	  CY_CHECK(cy_lib_uninit(&cryptolib));
	  Print_RAMp(Ramp, _MAX_MEMORY);

	  end:
	    if (error == CY_OK)
	      printf(" All tests OK !\n");
	    else
	      printf(" KO\n");
	    return error;

	  return 0;
}

