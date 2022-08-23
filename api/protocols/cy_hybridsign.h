/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: hybrid signatures APIS                                          */
/**********************************************************************************/

#ifndef CY_HYBRIDSIG_H_
#define CY_HYBRIDSIG_H_

#include "cy_sign.h"

/* todo*/

struct cy_hybridsig_ctx_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  uint64_t id; /**< Component identifier                                   */
  unsigned char Component_name[MAX_SIZENAME];

  /*Prequantum functions*/
  cy_sign_ctx_t *Prequantum_ctx
  FCT_Sign_Init_t   Prequantum_Init;
  FCT_Sign_Proove_t Prequantum_Proove;
  FCT_Sign_Verif_t  Prequantum_Verif;

  cy_sign_ctx_t *Postquantum_ctx
  FCT_Sign_Init_t   Postquantum_Init;
  FCT_Sign_Proove_t Postquantum_Proove;
  FCT_Sign_Verif_t  Postquantum_Verif;

};

typedef struct cy_hybridsig_ctx_s cy_hybridsig_ctx_t;

cy_error_t cy_hybridsig_init(cy_hybridsig_ctx_t *ctx,  sign_role_t role,
							 uint8_t *datainit, size_t m_t8,

							 );

cy_error_t cy_hybridsig_proove_all(cy_hybridsig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_hybridsig_proove_init(cy_hybridsig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_hybridsig_proove_update(cy_hybridsig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);
cy_error_t cy_hybridsig_proove_verify(cy_hybridsig_ctx_t *ctx, cy_sk_t *publickey, uint8_t *message, size_t m_t8, uint8_t *out);

cy_error_t cy_hybridsig_verify_all(cy_hybridsig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_hybridsig_verify_init(cy_hybridsig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_hybridsig_verify_update(cy_hybridsig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);
cy_error_t cy_hybridsig_verify_final(cy_hybridsig_ctx_t *ctx, cy_pk_t *secretkey, uint8_t *message, size_t m_t8, boolean_t *flagverif);

cy_error_t cy_hybridsig_uninit(cy_hybridsig_ctx_t *ctx,  uint8_t *message, size_t m_t8);


#endif /*CY_HYBRIDSIG_H_ */

