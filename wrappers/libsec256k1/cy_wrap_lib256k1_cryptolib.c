/*
 * cy_wrap_sec256k1.h
 *
 *  Created on: Aug 8, 2022
 *      Author: dubois
 */

#include <stddef.h>
#include <stdio.h>

#include "cy_errors.h"

#include "cy_wrap_fp_lib256k1.h"
#include "cy_fp.h"
#include "cy_cryptolib.h"
#include "cy_wrap_lib256k1_gda.h"
#include "cy_memory_unit.h"

#include "cy_wrap_lib256k1_cryptolib.h"

cy_error_t cy_wrap_lib256k1_init(cryptolib_ctx_t *i_cryptolib,
                              uint8_t *Shared_Memory, size_t allocated_t8,
                              uint8_t *additional, size_t additional_t8)
{

  cy_error_t error = CY_KO;

  if (allocated_t8 > _MAX_MEMORY) {
    goto end;
  }


  i_cryptolib->mem_unit = &lib256k1_mem_unit;

  /* Initialization of memory unit*/
  lib256k1_mem_unit.Shared_Memory = Shared_Memory;

  CY_CHECK(cy_mem_init(i_cryptolib->mem_unit, allocated_t8, 4));


  /* Initialization of gda*/
  i_cryptolib->gda = &lib256k1_gda_component;
  CY_CHECK(lib256k1_gda_component.GDA_Init(&lib256k1_gda_component, NULL, 0));

  UNUSED(additional);
  UNUSED(additional_t8);

  i_cryptolib->is_initialized = CY_LIB_INITIALIZED;

  error = CY_OK;
end:
  return error;
}

cy_error_t cy_wrap_lib256k1_uninit(cryptolib_ctx_t *i_cryptolib)
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
  (lib256k1_gda_component.GDA_UnInit(&lib256k1_gda_component));

  error = CY_OK;
end:
  return error;
}
