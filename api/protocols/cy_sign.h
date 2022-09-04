/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_sign.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: signature APIS		                                              */
/**********************************************************************************/

#ifndef SRC_CY_SIG_H_
#define SRC_CY_SIG_H_

#include <stddef.h>
#include <stdint.h>
#include "cy_errors.h"

/* todo*/
#define cy_sign_ctx_t void
#define cy_pk_t void
#define cy_sk_t void
#define sign_role_t void

cy_error_t cy_sign_init(cy_sign_ctx_t *ctx,  sign_role_t role, uint8_t *datainit, size_t m_t8);

/* a one shot computation of the Signature computation*/
cy_error_t cy_sign_proove_all(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *nonce, uint8_t *message, size_t m_t8, uint8_t *out);

/* classical triplet of function int, update, final */
cy_error_t cy_sign_proove_init(cy_sign_ctx_t *ctx, cy_sk_t *secretkey, uint8_t *nonce,uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_sign_proove_update(cy_sign_ctx_t *ctx, cy_sk_t *secretkey, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_sign_proove_verify_final(cy_sign_ctx_t *ctx, cy_sk_t *secretkey, uint8_t *message, size_t m_t8, uint8_t *out);

/* a one shot computation of the Signature verification*/
cy_error_t cy_sign_verify_all(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *message, size_t m_t8, boolean_t *flagverif);


/* classical triplet of function int, update, final */
cy_error_t cy_sign_verify_init(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_sign_verify_update(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_sign_verify_final(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *message, size_t m_t8, boolean_t *flagverif);

cy_error_t cy_sign_uninit(cy_sign_ctx_t *ctx,  uint8_t *message, size_t m_t8);



/* those pointers will be used for hybridation of sigs */
typedef cy_error_t (*FCT_Sign_Init_t)  (cy_sign_ctx_t *ctx,  sign_role_t role, uint8_t *datainit, size_t m_t8);
typedef cy_error_t (*FCT_Sign_Proove_t)(cy_sign_ctx_t *ctx, cy_pk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);
typedef cy_error_t (*FCT_Sign_Verif_t) (cy_sign_ctx_t *ctx, cy_sk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);


#endif /* SRC_ARITHMETIC_PB_CY_PB_H_ */
