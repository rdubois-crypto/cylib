#include "innovation/cx_mulmontg.h"

// static const limb_t p0 = (limb_t)0x89f3fffcfffcfffd;  /* -1/P */

#define _MAX_BN_T8 2048

const uint8_t BLS12_381_P[48] = {
  0x1a, 0x01, 0x11, 0xea, 0x39, 0x7f, 0xe6, 0x9a, 0x4b, 0x1b, 0xa7, 0xb6,
  0x43, 0x4b, 0xac, 0xd7, 0x64, 0x77, 0x4b, 0x84, 0xf3, 0x85, 0x12, 0xbf,
  0x67, 0x30, 0xd2, 0xa0, 0xf6, 0xb0, 0xf6, 0x24, 0x1e, 0xab, 0xff, 0xfe,
  0xb1, 0x53, 0xff, 0xff, 0xb9, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xaa, 0xab
};

static cx_err_t cy_io_bn_print_ByteString(cx_bn_t i_x, char *prefix,
                                          char *suffix)
{
  cx_err_t error;
  uint8_t tu8_print[_MAX_BN_T8];
  size_t t8_x;
  size_t i;

  CX_CHECK(sys_cx_bn_nbytes(i_x, &t8_x));
  CX_CHECK(sys_cx_bn_export(i_x, tu8_print,
                            t8_x)); /* todo: change this cast by proper call*/

  printf("%s\n", prefix);
  for (i = 0; i < t8_x - 1; i++) {
    printf("0x%02X, ", tu8_print[i]);
    if ((i & 0xf) == 15)
      printf("\n");
  }
  printf("0x%02X", tu8_print[i]);
  printf("\b%s", suffix);

end:
  return error;
}

static cx_err_t cy_io_bn_print_HexaString(cx_bn_t i_x, char *prefix,
                                          char *suffix)
{
  cx_err_t error;
  uint8_t tu8_print[_MAX_BN_T8];
  size_t t8_x;
  size_t i;

  CX_CHECK(sys_cx_bn_nbytes(i_x, &t8_x));
  CX_CHECK(sys_cx_bn_export(i_x, tu8_print,
                            t8_x)); /* todo: change this cast by proper call*/

  printf("%s", prefix);
  for (i = 0; i < t8_x; i++) {
    printf("%02X", tu8_print[i]);
  }
  printf("\b%s", suffix);

end:
  return error;
}

int add(int i, int j)
{
  int res = 0;
  __asm("ADD %[result], %[input_i], %[input_j]"
        : [ result ] "=r"(res)
        : [ input_i ] "r"(i), [ input_j ] "r"(j));
  return res;
}

static void WORD128_MULF(word_montgomery_t outh, word_montgomery_t outl,
                         word_montgomery_t in1, word_montgomery_t in2)
{

  uint128_t l_tmph, l_tmpm, l_tmpl;
  uint128_t l_tmpm1, l_tmpm2;
  uint128_t l_carryl;
  uint64_t l_carrym;

  /* Compute low product. */
  WORD64_MUL(l_tmpl, in2[1], in1[1]);
  /* Compute middle product with its l_carry. */
  WORD64_MUL(l_tmpm1, in2[0], in1[1]);
  WORD64_MUL(l_tmpm2, in2[1], in1[0]);
  WORD128_ADDC(l_tmpm, l_carrym, l_tmpm1, l_tmpm2);
  /* Compute full low product. */
  (outl[1]) = l_tmpl[1];
  (outl[0]) = l_tmpl[0] + l_tmpm[1]; /* Store full low product l_carry. */
  l_carryl[1] = ((outl[0]) < l_tmpl[1]);
  /* Compute full high product. */
  l_carryl[1] = (l_carryl[1] + (l_tmpm[0]));
  l_carryl[0] = (l_carryl[1] < (l_tmpm[0]));
  l_carryl[0] += (l_carrym);
  WORD64_MUL(l_tmph, in2[0], in1[0]);
  /* No l_carry can occur below. */
  WORD128_ADDC(outh, l_carrym, l_tmph, l_carryl);
}

int main()
{
  /* I. Declarations*/
  cx_err_t error = CX_INTERNAL_ERROR;
  cx_bn_t bn_p;
  cx_bn_t bn_a;
  cx_bn_mont_ctx_t ctx;
  word_montgomery_t res;
  word_montgomery_t res_hi, res_low;

  const word_montgomery_t var_a = { 0xB39EA4D2A7B86062, 0x7E73D28E29C2847F };
  const word_montgomery_t var_b = { 0xF2FEFFE52AC81FA1, 0x01CF9A929E10A774 };

  uint64_t c, d;
  uint128_t a;

  c = 0x8000000000000001;
  d = 2;
  // umul_ppmm(a, b,c,d);
  WORD64_MUL(a, c, d);
  printf("\n a_hi=%llx a_lo=%llx", a[0], a[1]);

  // WORD128_MULF(res_hi, res_low, var_a, var_b);
  WORD128_MUL(res_hi, res_low, var_a, var_b);
  printf("Mul 128x128:");
  printf("\n %llx %llx %llx %llx", res_hi[0], res_hi[1], res_low[0],
         res_low[1]);

  printf("\n size of int=%d", sizeof(int));

  /* II. Allocations*/
  CX_CHECK(sys_cx_bn_lock(48, 0)); /* Init BN*/
  printf("\n Test cx_mont_init with word size=%d", sizeof(word_montgomery_t)
                                                       << 3);

  CX_CHECK(sys_cx_bn_alloc_init(&bn_p, 48, BLS12_381_P, 48));
  CX_CHECK(sys_cx_bn_alloc_init(&bn_a, 48, BLS12_381_P, 48));
  CX_CHECK(cx_mont_alloc(&ctx, 48));

  CX_CHECK(cy_io_bn_print_ByteString(bn_p, "\n P:", "\n"));
  CX_CHECK(cy_io_bn_print_HexaString(bn_p, "\n P:", "\n"));
  CX_CHECK(cx_mont_init(&ctx, bn_p));

  CX_CHECK(cy_io_bn_print_HexaString(ctx.h, "\n h:", "\n"));

  cy_get_mont_register(&res);
  printf("\n res= %llx %llx", res[0], res[1]);

  CX_CHECK(sys_cx_bn_destroy(&bn_p));

end:
  printf("Return code:%x\n ", error);

  return error;
}

/* Magma Vector References computations for stark:
 * P:=StringToInteger("1A0111EA397FE69A4B1BA7B6434BACD764774B84F38512BF6730D2A0F6B0F6241EABFFFEB153FFFFB9FEFFFFFFFFAAAB",16);
 * Decimal :
4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559787
 * IntegerToString(2^384 mod P,16);
 * Result for
h:15F65EC3FA80E4935C071A97A256EC6D77CE5853705257455F48985753C758BAEBF4000BC40C0002760900000002FFFD
 *
 * P:=StringToInteger("B9FEFFFFFFFFAAAB",16);
 * Decimal : 13402431016077863595
 * Montgomery representations:
 * 1/P mod 2^64=Mod(13402431016077863595, 2^64)^(2^64-1)= 8506173809081122819=
0x760C000300030003
 * -1/P mod 2^64=  Mod(-13402431016077863595, 2^64)^(2^64-1)=9940570264628428797
= 0x89f3fffcfffcfffd
 * P:=StringToInteger("1EABFFFEB153FFFFB9FEFFFFFFFFAAAB",16)=40769914829639538012874174947278170795
 * 1/P mod 2^128=Mod(40769914829639538012874174947278170795, 2^128)^(2^128-1)=
=286558410140111203378102252004914167811 =D795246D262EEC17760C000300030003
 * -1/P mod 2^128=53723956780827260085272355426854043645 =
286ADB92D9D113E889F3FFFCFFFCFFFD
 *
a
238755537154144465039598555137428325503
B39EA4D2A7B860627E73D28E29C2847F
b
322997208552955939342696146542454155124
F2FEFFE52AC81FA101CF9A929E10A774
a*b
77117372027350220156783885706246345961074538768570589249316423458835927327372
AA7EE2C27CAA6AB9 EF4B030C74B643EE 1599805DDB0D262F 37D3C6057A82E28C

 *
 */
/* SAGE implementation for 256k1*/

p256k1 = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F

    Fp = FiniteField(p256k1) n0 =
        mod(-p256k1, 2 ^ 128) ^ -1 Rm1 = Fp(mod(2 ^ 128, p256k1) ^ -1) a =
            Fp.random_element() b = Fp.random_element() R = 2 ^ 128;

Ma = Fp(a * R) Mb = Fp(b * R)

    print("A*R=", hex(Ma)) print("B*R=", hex(Mb)) print(
        "A*B*R=", hex(a *b *R)) print("n0=", n0)

        A *R = 0x329efd7b3f30d8b2c2476c804506484dfcd150436a0cbf871ad7a1c78e2ac345 B
            *R = 0x49e541bf948579eb4e92ebf5e8f0f80f184c6db04df8670013dde86d17cdf296 A
                *B *R =
                    0xf3d1ecf7de8a9295 6ee05242b9e6342d 8a4be8f1965c3693 686e0ba06791184d abRpP =
                        0x1f3d1ecf7de8a92956ee05242b9e6342d8a4be8f1965c3693686e0b9f6791147c

                        (R - 1) *
                        (R -
                         1)0xfffffffffffffffffffffffffffffffe 0000000000000000 0000000000000001
