
#ifndef _CY_PBC_H
#define _CY_PBC_H


cx_err_t cy_pairing(cx_curve_t curve, cy_ecpoint_t *P1, cy_ecpoint_t *P2,
                    uint8_t *pairing, size_t size8_pairing);

cx_err_t cy_pairing_asn1(cx_curve_t curve, unsigned char *P1, size_t P1_len,
                         unsigned char *P2, size_t P2_len,
                         const unsigned char *k, unsigned int k_len);

#endif
