cmake_minimum_required(VERSION 3.14)
project(forward_list_test)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
	forward_list_test
	forward_list_test.cpp
)
target_link_libraries(
	forward_list_test
	gtest_main
)
add_compile_options(-g -Wall -Wextra)

include(GoogleTest)
gtest_discover_tests(forward_list_test)

target_include_directories(${PROJECT_NAME} PUBLIC ../ , ../../ , .)