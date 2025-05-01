# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(beman_execution REQUIRED)
find_package(GTest REQUIRED)

add_executable(beman.sequence_next.tests.sequence_next)
target_sources(
    beman.sequence_next.tests.sequence_next
    PRIVATE sequence_next.test.cpp
)
target_link_libraries(
    beman.sequence_next.tests.sequence_next
    PRIVATE beman::sequence_next beman::execution GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(beman.sequence_next.tests.sequence_next)
