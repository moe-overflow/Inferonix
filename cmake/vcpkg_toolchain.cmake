function(set_vcpkg_toolchain)
    if ("$ENV{VCPKG_ROOT}" STREQUAL "")
        message(FATAL_ERROR "FATAL: VCPKG_ROOT environment variable is not set!")
    endif()

    message(STATUS "VCPKG_ROOT environment variable is $ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

endfunction()
