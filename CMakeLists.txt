# Web Class Helper CMake Lists File 2.1.2
# This source code file is under MIT License.
# Copyright (c) 2022 - 2023 Class Tools Develop Team
# Contributors: ren-yc
cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

project(WCH)

add_executable(WCH "WCH.cpp" "version.hpp" "shasum.hpp" "modules/apis.hpp" "modules/basic.hpp" "modules/commands.hpp" "modules/file-process.hpp" "modules/functions.hpp" "modules/init.hpp" "WCH.rc" "WCH.manifest")
if(POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set_property(TARGET WCH PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set_property(TARGET WCH PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()
add_compile_definitions(UNICODE)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(WCH_Architecture="${ARCHITECTURE}")
target_compile_options(WCH PRIVATE /W3 /utf-8)

set_property(TARGET WCH PROPERTY CXX_STANDARD 20)

if(NOT DEFINED VCPKG_ROOT)
	if(DEFINED ENV{VCPKG_ROOT})
		set(VCPKG_ROOT ENV{VCPKG_ROOT})
	else()
		message(FATAL_ERROR "VCPKG_ROOT is not defined.")
	endif()
endif()

if(DEFINED ENV{CI})
	set(VCPKG_INSTALL_OPTIONS "--clean-after-build")
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
find_package(fmt CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(WCH fmt::fmt jsoncpp_static JsonCpp::JsonCpp OpenSSL::SSL OpenSSL::Crypto spdlog::spdlog)
