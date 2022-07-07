


static inline void mul_fp2(quad_ctx_t *ctx, quad_t *ret, const quad_t *a, const quad_t *b)
{
 cy_mult_mont(fp_ctx_t *ctx, fp_t *a, fp_t *b);
}

/*karatsuba like, assuming complex representation of quad extension*/
void mul_mont_384x(quad_t *ret, const quad_t *a, const quad_t *b,
                   const fp_t p, limb_t n0)
{
  vec384 aa, bb, cc;

  cy_fp_add(aa, a[0], a[1], p, NLIMBS(384));
  cy_fp_add(bb, b[0], b[1], p, NLIMBS(384));
  cy_fp_mult_mont(bb, bb, aa, p, n0, NLIMBS(384));
  cy_fp_mult_mont(aa, a[0], b[0], p, n0, NLIMBS(384));
  cy_fp_mult_mont(cc, a[1], b[1], p, n0, NLIMBS(384));
  cy_fp_sub(ret[0], aa, cc, p, NLIMBS(384));
  cy_fp_sub(ret[1], bb, aa, p, NLIMBS(384));
  cy_fp_sub(ret[1], ret[1], cc, p, NLIMBS(384));
}
