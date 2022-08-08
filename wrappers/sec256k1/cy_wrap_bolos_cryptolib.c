/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_lib.h */
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Bolos cryptolib wrapping */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"
#include "cy_cryptolib.h"
#include "cy_wrap_bolos_gda.h"
#include "cy_memory_unit.h"

#include "cy_wrap_bolos_cryptolib.h"

cy_error_t cy_wrap_bolos_init(cryptolib_ctx_t *i_cryptolib,
                              uint8_t *Shared_Memory, size_t allocated_t8,
                              uint8_t *additional, size_t additional_t8)
{
  cy_error_t error = CY_KO;

  if (allocated_t8 > _MAX_MEMORY) {
    goto end;
  }

  i_cryptolib->mem_unit = &bolos_mem_unit;

  /* Initialization of memory unit*/
  bolos_mem_unit.Shared_Memory = Shared_Memory;

  CY_CHECK(cy_mem_init(i_cryptolib->mem_unit, allocated_t8, 4));

  /* Initialization of gda*/
  i_cryptolib->gda = &bolos_gda_component;
  CY_CHECK(bolos_gda_component.GDA_Init(&bolos_gda_component, NULL, 0));

  UNUSED(additional);
  UNUSED(additional_t8);

  i_cryptolib->is_initialized = CY_LIB_INITIALIZED;

  error = CY_OK;
end:
  return error;
}

cy_error_t cy_wrap_bolos_uninit(cryptolib_ctx_t *i_cryptolib)
{
  cy_error_t error = CY_KO;

  /* UnInitialization of memory unit*/
  error=(cy_mem_uninit((i_cryptolib->mem_unit)));

  /* in case of any error, the memory is safely erased by calling the gda*/
  if(error!=CY_OK)
  {
	  i_cryptolib->gda->GDA_Run(i_cryptolib->gda, i_cryptolib->mem_unit->Shared_Memory, i_cryptolib->mem_unit->allocated_t8);
	  goto end;
  }
  /* UnInitialization of gda*/
  (bolos_gda_component.GDA_UnInit(&bolos_gda_component));

 // UNUSED(pu8_Mem);
 // UNUSED(t8_Memory);

  error = CY_OK;
end:
  return error;
}
