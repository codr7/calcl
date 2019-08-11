cmake_minimum_required(VERSION 3.5)

project(calcl)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-std=c++17 -Wall -Werror -O2 -g)

file(GLOB_RECURSE src src/calcl/*.cpp)

add_library(lib_shared SHARED ${src})
set_target_properties(lib_shared PROPERTIES OUTPUT_NAME calcl)
target_include_directories(lib_shared PUBLIC src/)

add_executable(calcl ${src} src/main.cpp)
target_include_directories(calcl PUBLIC src/)
target_link_libraries(calcl cidk stdc++fs)

file(GLOB root_inc src/calcl/*.hpp)
install(FILES ${root_inc} DESTINATION include/calcl)

install(FILES build/calcl DESTINATION bin PERMISSIONS WORLD_EXECUTE)
install(FILES build/libcalcl.a DESTINATION lib)
