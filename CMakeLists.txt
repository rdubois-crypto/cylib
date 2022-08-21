add_compile_options(-Wextra -Wcast-qual -Wshadow -Werror
        -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-security -Wformat-signedness
)

add_library(emu
		##############################################################
		# I. SPECULOS/BOLOS
		##############################################################
		
		#I.1) speculos (bolos emulation)
		
        bolos/cx_aes.c
        bolos/cx_aes_sdk2.c
        bolos/cx_blake2b.c
        bolos/cx_bn.c
        bolos/cx_curve25519.c
        bolos/cx_crc.c
        bolos/cx_ec.c
        bolos/cx_ec_domain.c
        bolos/cx_ecpoint.c
        bolos/cx_ed25519.c
        bolos/cx_hmac.c
        bolos/cx_hkdf.c
        bolos/cx_montgomery.c
        bolos/cx_mpi.c
        bolos/cx_ripemd160.c
        bolos/cx_rng_rfc6979.c
        bolos/cx_scc.c
        bolos/cx_sha256.c
        bolos/cx_sha3.c
        bolos/cx_sha512.c
        bolos/cx_twisted_edwards.c
        bolos/cx_utils.c
        bolos/cx_weierstrass.c
        bolos/cxlib.c
        bolos/bagl.c
        bolos/endorsement.c
        bolos/seproxyhal.c
        bolos/os.c
        bolos/os_1.2.c
        bolos/os_1.5.c
        bolos/os_1.6.c
        bolos/os_2.0.c
        bolos/os_bip32.c
        bolos/os_eip2333.c
        bolos/cx.c
        bolos/cx_hash.c
        bolos/cx_math.c
        bolos/default.c
        emulate.c
        emulate_1.2.c
        emulate_1.5.c
        emulate_1.6.c
        emulate_2.0.c
        emulate_blue_2.2.5.c
        emulate_lnsp_1.0.c
        
        #I.2) wrap bolos lib
    	innovation/wrappers/bolos/cy_wrap_bolos_cryptolib.c
     	innovation/wrappers/bolos/cy_wrap_bolos_gda.c
     	innovation/wrappers/bolos/cy_wrap_fp_bolos.c
     	
     	##############################################################
		# II. BLST
		##############################################################
		
     	#I.1) adapt blst sources to speculos environment constraints
     	
        	external/blst/blst_exp.c
        	external/blst/blst_vect.c
     		external/blst/blst_consts.c
        	external/blst/blst_sqrt.c
        	external/blst/blst_recip.c
        	external/blst/blst_e1.c
        	external/blst/blst_e2.c
     		external/blst/blst_fp12_tower.c
     		external/blst/blst_pairing.c
     	
     	# #I.2) wrap blst lib
        #innovation/cy_wrap_blst_pbc.c
        
     	##############################################################
		# III. LIB256K1
		##############################################################
	
     	#wrap sec256k1zkp lib
     	innovation/wrappers/libsec256k1/cy_wrap_lib256k1_gda.c
     	innovation/wrappers/libsec256k1/cy_wrap_lib256k1_cryptolib.c
     	innovation/wrappers/libsec256k1/cy_wrap_fp_lib256k1.c
     
     	innovation/components/cy_memory_unit.c
     	
     	##############################################################
		# IV. LibECC
		##############################################################
		external/libECC/nn/nn.c
		external/libECC/nn/nn_add.c
		external/libECC/nn/nn_div.c
		external/libECC/nn/nn_mul.c
		external/libECC/fp/fp.c
		
		innovation/wrappers/libECC/cy_wrap_fp_libECC.c
     	##############################################################
		# V. CY_LIB NATIVE
		##############################################################
		innovation/src/io/cy_io_common_tools.c
		innovation/src/io/cy_io_fp.c
		
		innovation/src/arithmetic/fp_extensions/cy_quad.c
		innovation/src/arithmetic/fp_extensions/cy_fp2x3x2.c
		innovation/src/arithmetic/pairings/cy_pbc.c
	
        svc.c)

add_dependencies(emu openssl)

add_executable(launcher launcher.c)
# ensure the loader .text section doesn't mess with cxlib
if (CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
  # Option for clang
  if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13.0")
    set_property(TARGET launcher APPEND PROPERTY LINK_OPTIONS "LINKER:--image-base=0xf0000000")
  else()
    set_property(TARGET launcher APPEND PROPERTY LINK_FLAGS "-Xlinker --image-base=0xf0000000")
  endif ()
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
  # Option for gcc
  if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13.0")
    set_property(TARGET launcher APPEND PROPERTY LINK_OPTIONS "LINKER:-Ttext-segment=0xf0000000")
  else()
    set_property(TARGET launcher APPEND PROPERTY LINK_FLAGS "-Wl,-Ttext-segment=0xf0000000")
  endif ()
else ()
  message(FATAL_ERROR "Unable to identify the compiler!")
endif ()
target_link_libraries(launcher PRIVATE emu)
