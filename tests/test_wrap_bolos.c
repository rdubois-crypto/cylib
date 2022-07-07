/*
 * Copyright Ledger
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */

#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/bn.h>
#include <openssl/ec.h>
#include <stdbool.h>
#include <stdio.h>

#include "cx_ec.h"
#include "cxlib.h"
#include "cy_errors.h"

#include "cy_wrap_bolos.h"
#include "cy_fp.h"

static uint8_t const secp384r1_p[] = {
  // p:
  // 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF0000000000000000FFFFFFFF
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
};


static uint8_t const secp384r1_t8[] ={48, 0, 0, 0, 0, 0, 0, 0};

int main()
{
	fp_ctx_t ctx;
	cy_error_t error;
	/* The shared ram between program and library*/
    uint8_t Ramp[_MAX_MEMORY];

    const uint8_t *argv[]={secp384r1_t8, secp384r1_p};

	printf("\n test bolos wrap:");
	error=wrap_bolos_fp_init(&ctx, Ramp , _MAX_MEMORY, 2, argv);

	if(error==CY_OK) printf(" OK\n");
	else printf(" KO\n");
	return error;
}
