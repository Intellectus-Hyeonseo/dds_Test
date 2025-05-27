
# Copyright 2024 Proyectos y Sistemas de Mantenimiento SL (Intellectus).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20)

project(int2dds_dds_test_example VERSION 1 LANGUAGES CXX)

if(NOT int2cdr_FOUND)
    find_package(int2cdr 2 REQUIRED)
endif()

if(NOT int2dds_FOUND)
    find_package(int2dds 3 REQUIRED)
endif()

set(OpenCV_DIR "C:/opencv/build") 
find_package(OpenCV REQUIRED)

#Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

# Set CMAKE_BUILD_TYPE to Release by default.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Configuring dds test example...")
file(GLOB DDS_TEST_SOURCES_CXX "*.cxx")
file(GLOB DDS_TEST_SOURCES_CPP "*.cpp")
file(GLOB DDS_TEST_TYPES_CXX "headers/*.cxx")
file(GLOB DDS_TEST_TYPES_CPP "headers/*.cpp")

# 모든 소스 합치기
set(DDS_TEST_SOURCES
        ${DDS_TEST_SOURCES_CXX}
        ${DDS_TEST_SOURCES_CPP}
        ${DDS_TEST_TYPES_CXX}
        ${DDS_TEST_TYPES_CPP}
)

add_executable(dds_test
        ${DDS_TEST_SOURCES}
)

target_compile_definitions(dds_test PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    $<$<BOOL:${SHM_TRANSPORT_DEFAULT}>:SHM_TRANSPORT_BUILTIN> # Enable SHM as built-in transport
)
target_include_directories(dds_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(dds_test PRIVATE int2dds int2cdr ${OpenCV_LIBS})

install(TARGETS dds_test
    RUNTIME DESTINATION ${DATA_INSTALL_DIR}/int2dds/examples/cpp/dds_test/${BIN_INSTALL_DIR})

# Compile option
if (MSVC)
    target_compile_options(dds_test PRIVATE /utf-8 /wd4828)
endif()

# Copy the XML files over to the build directory
file(GLOB_RECURSE XML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.xml)
# for each xml file detected
foreach(XML_FILE_COMPLETE_PATH ${XML_FILES})
    # obtain the file name
    get_filename_component(XML_FILE ${XML_FILE_COMPLETE_PATH} NAME_WE)
    # copy the file from src to build folders
    configure_file(
        ${XML_FILE_COMPLETE_PATH}                   # from full src path
        ${CMAKE_CURRENT_BINARY_DIR}/${XML_FILE}.xml # to relative build path
        COPYONLY)
    install(FILES ${XML_FILE_COMPLETE_PATH}
        DESTINATION ${DATA_INSTALL_DIR}/int2dds/examples/cpp/dds_test/${BIN_INSTALL_DIR})
endforeach()