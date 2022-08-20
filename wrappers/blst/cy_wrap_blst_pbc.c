#include <stddef.h>
#include <stdint.h>

#include "cy_errors.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_wrap_blst_pbc.h"

#include "blst_e1.h"
#include "blst_e2.h"
#include "blst_errors.h"
#include "blst_fields.h"
#include "blst_pairing.h"
#include "blst_point.h"


/* Computation of the bilinear map e(P1, P2) : G1xG2----->GT,
 * where G1 is a pairing friendly elliptic curve over field Fp of characteristic
 * p, G2 it's counterpart over p^(k/twist) and GT the extension field of degree
 * k over Fp. For the current implementation, p=p_BLS12_381, k=12 and twist=6
 * (sextic twist)
 */

/* using ASN1 representation, uncompressed point of 96 bytes where
 * x=[0..47], y=[48..95], msb representation
 * */
cx_err_t cy_pairing_asn1(cx_curve_t curve, unsigned char *P1, size_t P1_len,
                         unsigned char *P2, size_t P2_len,
                         const unsigned char *k, unsigned int k_len)
{
  cy_error_t error = CX_INTERNAL_ERROR;
  POINTonE1_affine ps_P1[1];
  POINTonE2_affine ps_P2[1];
  vec384fp12 pairing_res;

  /* only BLS12_381 is supported for pairing right now*/
  if (curve != CX_CURVE_BLS12_381_G1) {
    error = CX_EC_INVALID_CURVE;
    goto end;
  }
  if (P1 == NULL || P2 == NULL || k == NULL) {
    error = CX_INVALID_PARAMETER;
    goto end;
  }
  if (k_len == 0 || P1_len != 96 || P2_len != 192) {
    error = CX_INVALID_PARAMETER;
    goto end;
  }

  POINTonE1_Deserialize_BE(ps_P1, P1);
  POINTonE2_Deserialize_BE(ps_P2, P2);

  miller_loop_n(pairing_res, ps_P2, ps_P1, 1);
  final_exp(pairing_res, pairing_res);

end:
  return error;
}

/* using bolos point representation*/
cx_err_t cy_pairing(cx_curve_t curve, cx_ecpoint_t *P1, cx_ecpoint_t *P2,
                    uint8_t *pairing, size_t size8_pairing)
{
	cy_error_t error = CX_INTERNAL_ERROR;

  /* only BLS12_381 is supported for pairing right now*/
  if (curve != CX_CURVE_BLS12_381_G1) {
    error = CX_EC_INVALID_CURVE;
    goto end;
  }
  if (P1 == NULL || P2 == NULL || pairing == NULL) {
    error = CX_INVALID_PARAMETER;
    goto end;
  }
  if (size8_pairing == 0) {
    error = CX_INVALID_PARAMETER;
    goto end;
  }

end:
  return error;
}
