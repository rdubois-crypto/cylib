/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_lib.h
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Library initialization and management API                         */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"
#include "cy_cryptolib.h"
#include "cy_memory_unit.h"


cy_error_t cy_wrap_bolos_init(cryptolib_ctx_t *i_cryptolib,  uint8_t *Shared_Memory, size_t allocated_t8, uint8_t *additional, size_t additional_t8)
{
	cy_error_t error=CY_KO;

	/* Initialization of memory unit*/
	CY_CHECK(cy_mem_init(&(i_cryptolib->mem_unit), allocated_t8));
	i_cryptolib->mem_unit.Shared_Memory=Shared_Memory;
	/* Initialization of gda*/
	i_cryptolib->gda=bolos_gda_component;
	CY_CHECK(bolos_gda_component->GDA_Init(&bolos_gda_component, NULL, 0, NULL, 0));


	i_cryptolib->is_initialized=CY_LIB_INITIALIZED;

	error=CY_OK;
	end:
		return error;
}

cy_error_t cy_bolos_uninit(cryptolib_ctx_t *i_cryptolib){
	cy_error_t error=CY_KO;

	/* UnInitialization of memory unit*/
	CY_CHECK(cy_mem_uninit(&(i_cryptolib->mem_unit)));
	/* UnInitialization of gda*/
	CY_CHECK(bolos_gda_component->GDA_UnInit(&bolos_gda_component));


	error=CY_OK;
	end:
	  return error;
}





