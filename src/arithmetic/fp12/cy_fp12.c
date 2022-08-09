


static void mul_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
    vec384x t0, t1, t2, t3, t4, t5;

    mul_fp2(t0, a[0], b[0]);
    mul_fp2(t1, a[1], b[1]);
    mul_fp2(t2, a[2], b[2]);

    /* ret[0] = ((a1 + a2)*(b1 + b2) - a1*b1 - a2*b2)*(u+1) + a0*b0
              = (a1*b2 + a2*b1)*(u+1) + a0*b0 */
    add_fp2(t4, a[1], a[2]);
    add_fp2(t5, b[1], b[2]);
    mul_fp2(t3, t4, t5);
    sub_fp2(t3, t3, t1);
    sub_fp2(t3, t3, t2);
    mul_by_u_plus_1_fp2(t3, t3);
    /* add_fp2(ret[0], t3, t0); considering possible aliasing... */

    /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1 + a2*b2*(u+1)
              = a0*b1 + a1*b0 + a2*b2*(u+1) */
    add_fp2(t4, a[0], a[1]);
    add_fp2(t5, b[0], b[1]);
    mul_fp2(ret[1], t4, t5);
    sub_fp2(ret[1], ret[1], t0);
    sub_fp2(ret[1], ret[1], t1);
    mul_by_u_plus_1_fp2(t4, t2);
    add_fp2(ret[1], ret[1], t4);

    /* ret[2] = (a0 + a2)*(b0 + b2) - a0*b0 - a2*b2 + a1*b1
              = a0*b2 + a2*b0 + a1*b1 */
    add_fp2(t4, a[0], a[2]);
    add_fp2(t5, b[0], b[2]);
    mul_fp2(ret[2], t4, t5);
    sub_fp2(ret[2], ret[2], t0);
    sub_fp2(ret[2], ret[2], t2);
    add_fp2(ret[2], ret[2], t1);

    add_fp2(ret[0], t3, t0);    /* ... moved from above */
}

static void sqr_fp6(vec384fp6 ret, const vec384fp6 a)
{
    vec384x s0, m01, m12, s2;

    sqr_fp2(s0, a[0]);

    mul_fp2(m01, a[0], a[1]);
    add_fp2(m01, m01, m01);

    mul_fp2(m12, a[1], a[2]);
    add_fp2(m12, m12, m12);

    sqr_fp2(s2, a[2]);

    /* ret[2] = (a0 + a1 + a2)^2 - a0^2 - a2^2 - 2*(a0*a1) - 2*(a1*a2)
              = a1^2 + 2*(a0*a2) */
    add_fp2(ret[2], a[2], a[1]);
    add_fp2(ret[2], ret[2], a[0]);
    sqr_fp2(ret[2], ret[2]);
    sub_fp2(ret[2], ret[2], s0);
    sub_fp2(ret[2], ret[2], s2);
    sub_fp2(ret[2], ret[2], m01);
    sub_fp2(ret[2], ret[2], m12);

    /* ret[0] = a0^2 + 2*(a1*a2)*(u+1) */
    mul_by_u_plus_1_fp2(ret[0], m12);
    add_fp2(ret[0], ret[0], s0);

    /* ret[1] = a2^2*(u+1) + 2*(a0*a1) */
    mul_by_u_plus_1_fp2(ret[1], s2);
    add_fp2(ret[1], ret[1], m01);
}

static void add_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
    add_fp2(ret[0], a[0], b[0]);
    add_fp2(ret[1], a[1], b[1]);
    add_fp2(ret[2], a[2], b[2]);
}

static void sub_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
    sub_fp2(ret[0], a[0], b[0]);
    sub_fp2(ret[1], a[1], b[1]);
    sub_fp2(ret[2], a[2], b[2]);
}

static void neg_fp6(vec384fp6 ret, const vec384fp6 a)
{
    neg_fp2(ret[0], a[0]);
    neg_fp2(ret[1], a[1]);
    neg_fp2(ret[2], a[2]);
}
