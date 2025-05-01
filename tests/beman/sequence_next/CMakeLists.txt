# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(beman_execution REQUIRED)
find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(beman.sequence_next.tests)
target_sources(
    beman.sequence_next.tests
    PRIVATE sequence_next.test.cpp state_helper.test.cpp
)
target_link_libraries(
    beman.sequence_next.tests
    PRIVATE beman::sequence_next beman::execution GTest::gtest_main
)

gtest_discover_tests(beman.sequence_next.tests)
