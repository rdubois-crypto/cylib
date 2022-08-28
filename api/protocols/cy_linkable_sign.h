/**********************************************************************************/
/* Copyright (C) 2022 - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_linksig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: linkable signatures APIS                                          */
/**********************************************************************************/

#ifndef SRC_CY_LINKSIG_H_
#define SRC_CY_LINKSIG_H_

/* todo*/
#define cy_linksigkey_ctx_t void
#define cy_pk_t void
#define cy_sk_t void
#define linksig_role_t void

cy_error_t cy_linksig_init(cy_linksig_ctx_t *ctx,  linksig_role_t role, uint8_t *datainit, size_t m_t8);
cy_error_t cy_linksig_uninit(cy_linksig_ctx_t *ctx,  uint8_t *message, size_t m_t8);

cy_error_t cy_linksig_proove_all(cy_linksig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);

cy_error_t cy_linksig_proove_init(cy_linksig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *nonce, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_linksig_proove_update(cy_linksig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_linksig_proove_verify(cy_linksig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);

cy_error_t cy_linksig_verify_all(cy_linksig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_linksig_verify_init(cy_linksig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_linksig_verify_update(cy_linksig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_linksig_verify_final(cy_linksig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);

cy_error_t cy_linksig_link(cy_linksig_ctx_t *ctx, cy_sk_t *secretkey, uint8_t *message, size_t m_t8, uint8_t *sig1, uint8_t *sig2, boolean_t *islinked);


#endif /*LINKSIG */

