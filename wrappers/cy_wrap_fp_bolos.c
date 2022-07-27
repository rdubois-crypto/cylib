#include <stddef.h>

#include <stdio.h>

#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"

/* The syntax of a bolos initializer is
 *
 * argv[0]:[0][1][2]...[WordByteSize-1]: bytesize of modulus
 * argv[1]:[WordByteSize...] : MSB asn1 encoding of modulus
 *
 * memory mapping of the library
 * 0: cx_bn_t value of modulus (address in SE)
 * 1: modulus, MSB encoding
 */

#define word32_from_be(a) (a[0]+(a[1]<<8)+(a[2]<<16)+(a[3]<<24) )

cy_error_t wrap_bolos_fp_init(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory,
		const int argc, const uint8_t *argv[]){

	cy_error_t error=CY_KO;
	size_t i;
	size_t offset=0;

	if(argc!=2) {
		return CY_KO;
	}
	/*control memory limit*/
	if(t8_Memory>_MAX_MEMORY){
		error=CY_KO;
		goto end;
	}

	ps_ctx->Shared_Memory=pu8_Mem; /*set Memory*/

	cx_bn_t *bn_p=(cx_bn_t *)ps_ctx->Shared_Memory;
    printf("\n @bn_p=%x",  (unsigned int) bn_p);

	offset+=sizeof(cx_bn_t *);


	strcpy(ps_ctx->libname, BOLOS_LIBNAME);
	ps_ctx->is_initialized=CY_LIB_INITIALIZED;


	ps_ctx->t8_modular=word32_from_be(argv[0]);
	printf("\n modular size=%d", (int) ps_ctx->t8_modular);

	for(i=0;i<ps_ctx->t8_modular;i++) pu8_Mem[i+offset]=argv[1][i];
	offset+=i;

	/* todo: endian independant read of argv0*/
	if(sys_cx_bn_lock(ps_ctx->t8_modular, 0)!=CX_OK){
		return CY_KO;
	}
	printf("\n locked");

    if(sys_cx_bn_alloc_init( bn_p, ps_ctx->t8_modular, argv[1], ps_ctx->t8_modular)!=CX_OK) /* allocating and initializing big number a*/
    {
    	error=CY_ERR_ALLOC;
    	goto end;
    }
    printf("\n bn_p=%x", (unsigned int) *bn_p);


    ps_ctx->modular=(void*) bn_p;

    ps_ctx->is_initialized=CY_LIB_INITIALIZED;
	error=CY_OK;
	end:
	return error;
}

/* TODO: developp a container to import/export ciphered containers
cy_error_t wrap_bolos_fp_write(fp_ctx_t *ctx, uint8_t *Mem __attribute__((unused)), int argc, uint8_t **argv)
{


}


cy_error_t wrap_bolos_fp_read(fp_ctx_t *ctx, uint8_t *Mem __attribute__((unused)), int argc, uint8_t **argv)
{


}
*/

cy_error_t wrap_bolos_fp_alloc(fp_ctx_t *ctx ,  size_t t8_r, cy_fp_t *r){
 cy_error_t error=CY_KO;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;
	 goto end;
 }

 CX_CHECK(sys_cx_bn_alloc(r, t8_r));

 error=CY_OK;

 end:
  return error;
}

cy_error_t wrap_bolos_fp_free(cy_fp_t *r)
{
 return(sys_cx_bn_destroy(r));
}

cy_error_t wrap_bolos_fp_import(fp_ctx_t *ctx ,  uint8_t *in, size_t t8_in, cy_fp_t *out){
	cy_error_t error=CY_KO;

	 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
		 error=CY_ERR_UNIT;
		 goto end;
	 }

	 CX_CHECK(sys_cx_bn_init(*out, in, t8_in));

	 error=CY_OK;

	 end:
	  return error;

}


cy_error_t wrap_bolos_fp_add(fp_ctx_t *ctx ,  cy_fp_t *a, cy_fp_t *b, cy_fp_t *r){
 cy_error_t error=CY_KO;

 if(ctx->is_initialized!=CY_LIB_INITIALIZED){
	 error=CY_ERR_UNIT;

	 goto end;
 }

 CX_CHECK(sys_cx_bn_mod_add(*r,*a,*b, *((cx_bn_t *) ctx->modular)));

 error=CY_OK;

 end:
  return error;
}


cy_error_t wrap_bolos_fp_uninit(fp_ctx_t *ps_ctx, uint8_t *pu8_Mem , const size_t t8_Memory)
{
	cy_error_t error=CY_KO;

	size_t i;

	for(i=0;i<t8_Memory;i++){
		pu8_Mem[i]=0x55;}

	CX_CHECK(sys_cx_bn_unlock());

	UNUSED(ps_ctx);

	 end:
	  return error;
}




