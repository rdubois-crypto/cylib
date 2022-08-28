/*
 * cy_linksig_Tryptich.h
 *
 *  Created on: Aug 24, 2022
 *      Author: dubois
 */

#ifndef CY_LINKSIG_TRYPTICH_H_
#define CY_LINKSIG_TRYPTICH_H_


/* Constante n and m of protocol set to 2 and 10 for 2^10 Ring size */
#define n 2
#define m 10

typedef enum
{
    READY,
    PLINK_WAIT,
    DATA_WAIT,
	UPDATE_END,

} tryptych_state;

struct cy_tryptich_ctx_s {
	ec_ctx_t ec_ctx;
	tryptych_state state;
	cy_gda_ctx_t *gda;
    void *additional;
    /* three generators with proven unknown relative discrete log*/
    cy_ec_point_t G;
    cy_ec_point_t H;
    cy_ec_point_t U;

};


typedef struct cy_tryptich_ctx_s cy_tryptic_ctx_t;


struct cy_tryptich_key_s {
	cy_fp_t fp_sk;
	unsigned int l; /* the 'identity index' in the ring */
	unsigned int mat_sigma[m][n];
	unsigned int mat_one_minus_2sigma[m][n]; /* precomputing once and for all (1-2sigma) for signing*/
};



typedef struct cy_tryptich_key_s cy_tryptic_sk_t;




#endif /* SRC_INNOVATION_SRC_PROTOCOLS_CY_LINKSIG_TRYPTICH_H_ */
