# cylib
An attempt to merge several different cryptographic libs into unified APIs:
- bolos cxlib (ledger)
- libECC (ANSSI)
- lib256k1zkp (with optional modules)
- blst
- relic
- sodium
- bearssl
- openssl
- oqs
- etc.

## Why another cryptolib ?
The framework is mainly intended to enable to switch primitives and accelerators (developper API) provided to higher level services (protocols and App, such as messenging, TLS, blockchain, NIST validation). The main observation comes from the fact that with unified API, the number of wrapper of n objects together would be reduced from O(n^2) to O(n) wrappers. Having unified API will also allow a unique binding from the API to rust (bind only once). 

![cylib](https://user-images.githubusercontent.com/103030189/178018785-ea4ea373-3eff-418f-a897-f205440740b8.png)

 Of course i don't want to let all the fun to others and some implementations are provided as 'native' CY_LIB implementations. I do not pretend to reach the skill of some of our best 'cryptodev wizards'. Every code that is reused is clearly indicated in the related source file by the 'acknowledgment' mention.

## How to make a good burritos ? (integrate a library or an accelerator in CY_LIB) ?

|  API file | Description      |     Possible Wrap           |     Type    |
|:---------:|------------------|:---------------------------:|:-----------:|
|  cy_fp.h  | Prime fields     | cy_wrap_bolos_fp.c          |  WRAP       |  
|           |                  | cy_wrap_bolos_fp.c          |  WRAP       |  
|  cy_fp2.h | Quadratic fields | cy_wrap_blst_fp2.c          |  WRAP       |
|           | Quadratic fields | cy_quad.c                   |  NATIVE     |
| cy_fp12.h | Dodecaic fields  | cy_wrap_blst_fp12.c         |  WRAP       |
|           |                  | cy_2x3x2.c                  |  NATIVE     |

Most of cryptographic accelerators implement modular field accelerations. This function shall be wrapped as done for instance in cy_wrap_bolos_* files. For NDA reasons it is not possible to publish examples over the NESCRYPT (STM secure element), but it is an example of such cryptocoprocessor.

According to the lib, various level of integration into CY_LIB is possible. One could choose to benefit from asm efficient prime field/elliptic curve implementation to benefit from high performances, then publish a code that could be reused by some HW manufacturer. The framework shall later enable fair performances benches of protocols. For instance, bolos provides elliptic curve computation, so is wrap at the cy_ec.h level, then reused to accelerate extensions fields computations. blst provides pairing computation over bls12, so can be directly mapped into cy_pairing.h level to build zkp protocols.

## Roadmap

### Current priorities
- finalize testing of prime fields integration of lib256k1 and bolos
- Map bls12_381 blst extension fields and pairing computations into cy_fp2.h, cy_fp12.h and cy_pairing.h
### TODO's 
- add emulations of vectorized integers in arm style (uint8x4x32_t) as wrappers such as neon2sse.h and sse2neon.h 
- generic Montgomery multiplier, adapted to any wordsize, including vectorized types and 128 bits multiplier (some hardware accelerators provide it).
- road to zkp: generic pairings and polynomial commitments.

## Needed
- An automated tool like clang-format, with only one possible output indentation (clang-format allow different representation of arrays, leading to possible git conflicts)

## Validation Strategy
Ideally a sagemath implementation for each function shall be provided. When possible, a wrapper to NIST test vectors shall be provided.
At some point this means that wrapping to the framework shall provide NIST CAVP without further effort to the primitives beneath the wrapper.

## Copyright and license
Copyright (C) 2017-2022

This software is licensed under a dual BSD and GPL v2 license. See LICENSE file at the root folder of the project.

## Contributing
Please contact me.

## Related projects

- [LIBECC] https://github.com/ANSSI-FR/libecc : a generic ecc and general purpose cryptographic library developped by the french ANSSI
- https://github.com/DLTcollab/sse2neon : A C/C++ header file that converts Intel SSE intrinsics to Arm/Aarch64 NEON intrinsics.
- [SEC256K1-ZKP] https://github.com/ElementsProject/secp256k1-zkp : Optimized C library for ECDSA signatures and secret/public key operations on curve secp256k1.
- [SPECULOS] https://github.com/LedgerHQ/speculos : an emulator of the open device Nano which enables to developp for a secure element from Ledger
- [BLST] https://github.com/supranational/blst : implementation of bls12-381 curve
- https://github.com/intel/ARM_NEON_2_x86_SSE/blob/master/NEON_2_SSE.h :A C/C++ header file that converts Arm/Aarch64 NEON intrinsics to Intel SSE intrinsics .
- [NESCRYPT] https://www.datasheetarchive.com/NESCRYPT-datasheet.html

