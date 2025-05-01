cmake_minimum_required(VERSION 3.24)

include(FetchContent)

if(NOT BEMAN_SEQUENCE_NEXT_LOCKFILE)
    set(BEMAN_SEQUENCE_NEXT_LOCKFILE
        "lockfile.json"
        CACHE FILEPATH
        "Path to the dependency lockfile for the Beman Sequence Next."
    )
endif()

set(BemanSequenceNext_projectDir "${CMAKE_CURRENT_LIST_DIR}/..")
message(
    TRACE
    "BemanSequenceNext_projectDir=\"${BemanSequenceNext_projectDir}\""
)

message(
    TRACE
    "BEMAN_SEQUENCE_NEXT_LOCKFILE=\"${BEMAN_SEQUENCE_NEXT_LOCKFILE}\""
)
file(
    REAL_PATH
    "${BEMAN_SEQUENCE_NEXT_LOCKFILE}"
    BemanSequenceNext_lockfile
    BASE_DIRECTORY "${BemanSequenceNext_projectDir}"
    EXPAND_TILDE
)
message(DEBUG "Using lockfile: \"${BemanSequenceNext_lockfile}\"")

# Force CMake to reconfigure the project if the lockfile changes
set_property(
    DIRECTORY "${BemanSequenceNext_projectDir}"
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS "${BemanSequenceNext_lockfile}"
)

# For more on the protocol for this function, see:
# https://cmake.org/cmake/help/latest/command/cmake_language.html#provider-commands
function(BemanSequenceNext_provideDependency method package_name)
    # Read the lockfile
    file(READ "${BemanSequenceNext_lockfile}" BemanSequenceNext_rootObj)

    # Get the "dependencies" field and store it in BemanSequenceNext_dependenciesObj
    string(
        JSON
        BemanSequenceNext_dependenciesObj
        ERROR_VARIABLE BemanSequenceNext_error
        GET "${BemanSequenceNext_rootObj}"
        "dependencies"
    )
    if(BemanSequenceNext_error)
        message(
            FATAL_ERROR
            "${BemanSequenceNext_lockfile}: ${BemanSequenceNext_error}"
        )
    endif()

    # Get the length of the libraries array and store it in BemanSequenceNext_dependenciesObj
    string(
        JSON
        BemanSequenceNext_numDependencies
        ERROR_VARIABLE BemanSequenceNext_error
        LENGTH "${BemanSequenceNext_dependenciesObj}"
    )
    if(BemanSequenceNext_error)
        message(
            FATAL_ERROR
            "${BemanSequenceNext_lockfile}: ${BemanSequenceNext_error}"
    )
    endif()

    # Loop over each dependency object
    math(
        EXPR
        BemanSequenceNext_maxIndex
        "${BemanSequenceNext_numDependencies} - 1"
    )
    foreach(BemanSequenceNext_index RANGE "${BemanSequenceNext_maxIndex}")
        set(BemanSequenceNext_errorPrefix
            "${BemanSequenceNext_lockfile}, dependency ${BemanSequenceNext_index}"
        )

        # Get the dependency object at BemanSequenceNext_index
        # and store it in BemanSequenceNext_depObj
        string(
            JSON
            BemanSequenceNext_depObj
            ERROR_VARIABLE BemanSequenceNext_error
            GET "${BemanSequenceNext_dependenciesObj}"
            "${BemanSequenceNext_index}"
        )
        if(BemanSequenceNext_error)
            message(
                FATAL_ERROR
                "${BemanSequenceNext_errorPrefix}: ${BemanSequenceNext_error}"
            )
        endif()

        # Get the "name" field and store it in BemanSequenceNext_name
        string(
            JSON
            BemanSequenceNext_name
            ERROR_VARIABLE BemanSequenceNext_error
            GET "${BemanSequenceNext_depObj}"
            "name"
        )
        if(BemanSequenceNext_error)
            message(
                FATAL_ERROR
                "${BemanSequenceNext_errorPrefix}: ${BemanSequenceNext_error}"
            )
        endif()

        # Get the "package_name" field and store it in BemanSequenceNext_pkgName
        string(
            JSON
            BemanSequenceNext_pkgName
            ERROR_VARIABLE BemanSequenceNext_error
            GET "${BemanSequenceNext_depObj}"
            "package_name"
        )
        if(BemanSequenceNext_error)
            message(
                FATAL_ERROR
                "${BemanSequenceNext_errorPrefix}: ${BemanSequenceNext_error}"
            )
        endif()

        # Get the "git_repository" field and store it in BemanSequenceNext_repo
        string(
            JSON
            BemanSequenceNext_repo
            ERROR_VARIABLE BemanSequenceNext_error
            GET "${BemanSequenceNext_depObj}"
            "git_repository"
        )
        if(BemanSequenceNext_error)
            message(
                FATAL_ERROR
                "${BemanSequenceNext_errorPrefix}: ${BemanSequenceNext_error}"
            )
        endif()

        # Get the "git_tag" field and store it in BemanSequenceNext_tag
        string(
            JSON
            BemanSequenceNext_tag
            ERROR_VARIABLE BemanSequenceNext_error
            GET "${BemanSequenceNext_depObj}"
            "git_tag"
        )
        if(BemanSequenceNext_error)
            message(
                FATAL_ERROR
                "${BemanSequenceNext_errorPrefix}: ${BemanSequenceNext_error}"
            )
        endif()

        if(method STREQUAL "FIND_PACKAGE")
            if(package_name STREQUAL BemanSequenceNext_pkgName)
                string(
                    APPEND
                    BemanSequenceNext_debug
                    "Redirecting find_package calls for ${BemanSequenceNext_pkgName} "
                    "to FetchContent logic fetching ${BemanSequenceNext_repo} at "
                    "${BemanSequenceNext_tag} according to ${BemanSequenceNext_lockfile}."
                )
                message(DEBUG "${BemanSequenceNext_debug}")
                FetchContent_Declare(
                    "${BemanSequenceNext_name}"
                    GIT_REPOSITORY "${BemanSequenceNext_repo}"
                    GIT_TAG "${BemanSequenceNext_tag}"
                    EXCLUDE_FROM_ALL
                )
                set(INSTALL_GTEST OFF) # Disable GoogleTest installation
                FetchContent_MakeAvailable("${BemanSequenceNext_name}")

                # Important! <PackageName>_FOUND tells CMake that `find_package` is
                # not needed for this package anymore
                set("${BemanSequenceNext_pkgName}_FOUND" TRUE PARENT_SCOPE)
            endif()
        endif()
    endforeach()
endfunction()

cmake_language(
    SET_DEPENDENCY_PROVIDER BemanSequenceNext_provideDependency
    SUPPORTED_METHODS FIND_PACKAGE
)
