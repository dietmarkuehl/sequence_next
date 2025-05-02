# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(beman_execution REQUIRED)

set(ALL_EXAMPLES hello-sequence_next)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.sequence_next.examples.${example})
    target_sources(
        beman.sequence_next.examples.${example}
        PRIVATE ${example}.cpp
    )
    target_link_libraries(
        beman.sequence_next.examples.${example}
        PRIVATE beman::sequence_next beman::execution
    )
    add_test(
        NAME beman.sequence_next.examples.${example}
        COMMAND $<TARGET_FILE:beman.sequence_next.examples.${example}>
    )
endforeach()
