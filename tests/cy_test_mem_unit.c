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
/* DESCRIPTION: testing wrappers for memory unit                       */
/**********************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cy_def.h"
#include "cy_errors.h"
#include "cy_memory_unit.h"
#include "cy_gda_component.h"


#if defined(_BOLOS_BURRITOS) || !defined(_TEST_ALL)
#include "innovation/cy_wrap_bolos_gda.h"
#include "innovation/cy_wrap_bolos_cryptolib.h"
#endif


#include "cy_cryptolib.h"
#include "cy_io_common_tools.h"

#define _EXAMPLE_T8 32

int test_mem_minimal_example(cryptolib_ctx_t *i_cryptolib)
{
  cy_error_t error = CY_OK;
  /* The shared ram between program and library*/

  uint8_t *FpZone;
  uint8_t *Ramp=i_cryptolib->mem_unit->Shared_Memory;
  UNUSED(Ramp);/* counter warning if no debug*/

  printf("\n test_mem_minimal_example:");

  debug_printf("\n Allocating First Fp component space, @RAMP=%x", (unsigned int)i_cryptolib->mem_unit->Shared_Memory);
  CY_CHECK(cy_mem_malloc(i_cryptolib->mem_unit, _EXAMPLE_T8, &FpZone));
  debug_printf("\n returned malloc @=%x", (unsigned int) FpZone);

  debug_Print_RAMp(Ramp,i_cryptolib->mem_unit->allocated_t8 );

  debug_printf("\n Freeing First Fp component space, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_mem_free(i_cryptolib->mem_unit, FpZone, _EXAMPLE_T8));

  debug_Print_RAMp(Ramp,i_cryptolib->mem_unit->allocated_t8 );


end:

  if (error == CY_OK)
    printf(" OK !\n");
  else
    printf(" KO\n");
  return error;
}

int test_mem_unit(cryptolib_ctx_t *i_cryptolib)
{
	cy_error_t error = CY_OK;
	printf("\n/************************ Test Memory Unit:");

	CY_CHECK(test_mem_minimal_example(i_cryptolib));

	end:
	 return error;
}

#ifndef _TEST_ALL


int main()
{
  cy_error_t error = CY_OK;
  uint8_t Ramp[_MAX_MEMORY];

  cryptolib_ctx_t cryptolib_bolos;


  debug_printf("\n @RAMP=%x", (unsigned int)Ramp);
  Print_RAMp(Ramp, _MAX_MEMORY);


  debug_printf("\n Starting lib, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_lib_init(&cryptolib_bolos, Ramp, sizeof(Ramp), NULL, 0));
  debug_Print_RAMp(Ramp, _MAX_MEMORY);

  test_mem_unit(&cryptolib_bolos);

  debug_printf("\n Closing lib, @RAMP=%x", (unsigned int)Ramp);
  CY_CHECK(cy_lib_uninit(&cryptolib_bolos));
  debug_Print_RAMp(Ramp, _MAX_MEMORY);

  end:
    if (error == CY_OK)
      printf(" All tests OK !\n");
    else
      printf(" KO\n");
    return error;

  return 0;
}

#endif
