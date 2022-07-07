/*
 * cy_wrap_blst.c
 *
 *  Created on: Jul 5, 2022
 *      Author: dubois
 */
#include "cy_errors.h"
#include "cy_wrap_blst.h"
#include "blst_fields.h"

cy_error_t wrap_blst_fp_add(fp_ctx_t *ctx,  *a, *b, *r){

 CY_UNUSED(ctx);
 add_fp(r, a, b);
 return CY_OK;
}

cy_error_t wrap_blst_fp_sub(fp_ctx_t *ctx,  *a, *b, *r){

 CY_UNUSED(ctx);
 sub_fp(r, a, b);
 return CY_OK;
}

cy_error_t wrap_blst_fp_mult_mont(fp_ctx_t *ctx,  *a, *b, *r){

 CY_UNUSED(ctx);
 mul_fp(r, a, b);

 return CY_OK;
}

