add_executable(main ${CMAKE_CURRENT_LIST_DIR}/main.c)
target_link_libraries(main PRIVATE m result)
target_compile_options(main PRIVATE -Wno-incompatible-pointer-types)
