/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_hashpedersen.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing hash pedersen functions */
/**********************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cy_errors.h"


#include "cy_configuration.h"
#include "cy_errors.h"
#include "cy_fp.h"

#include "cy_ec.h"

#include "cy_io_common_tools.h"
#include "cy_HashPedersen.h"

int test_verif_pedersen(cy_ec_ctx_t *ec_ctx)
{

 return 0;
}

int test_pedersen()
{

	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;
	uint8_t Ramp[ _EC_ZONE_T8 ];
	size_t nb_users=4;

	/* Initiate elliptic structure*/
	CY_CHECK(cy_ec_init(&ec_ctx, Ramp,_EC_ZONE_T8, CY_CURVE_Stark256, NULL));

	printf("\n\n /************************ Test Pedersen Hash:");
		error=test_verif_pedersen(&ec_ctx);


	end:
	  return error;
}
