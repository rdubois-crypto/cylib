add_definitions(-DST31)

link_libraries(emu -lcmocka-static)

#add_executable(test_pbc cy_test_pbc.c)


#to compile only sub modules, you must comment test_libs_all, then uncomment targeted submodules
#add_executable(test_mem inno/cy_test_mem_unit.c)

#compilation of all submodules, comment if you wanna test only some submodules
add_executable(teste_cylib_all cy_test_libs_all.c)


foreach(target test_swu_hash)
    add_test(NAME ${target} COMMAND qemu-arm-static ${target} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
