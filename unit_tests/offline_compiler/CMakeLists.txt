# Copyright (c) 2018, Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

project(cloc_tests)

set(IGDRCL_SRCS_cloc
  ${IGDRCL_SOURCE_DIR}/offline_compiler/offline_compiler.cpp
)

set(IGDRCL_SRCS_offline_compiler_mock
  ${CMAKE_CURRENT_SOURCE_DIR}/mock/mock_offline_compiler.h
)

set(IGDRCL_SRCS_offline_compiler_tests
  ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
  ${CMAKE_CURRENT_SOURCE_DIR}/environment.h
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_compiler_tests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/offline_compiler_tests.h
  ${IGDRCL_SOURCE_DIR}/runtime/helpers/abort.cpp
  ${IGDRCL_SOURCE_DIR}/runtime/helpers/file_io.cpp
  ${IGDRCL_SOURCE_DIR}/runtime/memory_manager/deferred_deleter.cpp
  ${IGDRCL_SOURCE_DIR}/runtime/memory_manager/deferred_deleter.h
  ${IGDRCL_SOURCE_DIR}/unit_tests/helpers/test_files.cpp
  ${IGDRCL_SRCS_cloc}
  ${IGDRCL_SRCS_offline_compiler_mock}
  ${CLOC_SRCS_LIB}
)

link_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(cloc_tests ${IGDRCL_SRCS_offline_compiler_tests})

target_include_directories(cloc_tests PRIVATE
  $<TARGET_PROPERTY:cloc,INCLUDE_DIRECTORIES>
)
target_compile_definitions(cloc_tests PUBLIC MOCKABLE_VIRTUAL=virtual $<TARGET_PROPERTY:cloc,INTERFACE_COMPILE_DEFINITIONS>)

target_link_libraries(cloc_tests igdrcl_mocks gmock-gtest elflib)

if(UNIX)
  target_link_libraries(cloc_tests dl pthread)
endif()

get_property(CLOC_FOLDER TARGET cloc PROPERTY FOLDER)
get_property(CLOC_COMPILE_FLAGS TARGET cloc PROPERTY COMPILE_FLAGS)
set_property(TARGET cloc_tests PROPERTY FOLDER ${CLOC_FOLDER})
set_property(TARGET cloc_tests APPEND_STRING PROPERTY COMPILE_FLAGS ${CLOC_COMPILE_FLAGS})

string(TOLOWER ${DEFAULT_TESTED_PLATFORM} CLOC_DEFAULT_DEVICE)

add_custom_target(run_cloc_tests ALL
  DEPENDS cloc_tests
)
macro(macro_for_each_platform)
  if("${PLATFORM_IT_LOWER}" STREQUAL "${CLOC_DEFAULT_DEVICE}")
    foreach(PLATFORM_TYPE "CORE" "LP")
      if(${PLATFORM_IT}_IS_${PLATFORM_TYPE})
        get_family_name_with_type(${GEN_TYPE} ${PLATFORM_TYPE})
        add_dependencies(run_cloc_tests test_kernels_${family_name_with_type})
        neo_copy_test_files(copy_test_files_${family_name_with_type} ${family_name_with_type})
        add_dependencies(run_cloc_tests copy_test_files_${family_name_with_type})
        set(run_tests_cmd cloc_tests --device ${CLOC_DEFAULT_DEVICE} --family_type ${family_name_with_type})
      endif()
    endforeach()
  endif()
endmacro()
macro(macro_for_each_gen)
  apply_macro_for_each_platform()
endmacro()

apply_macro_for_each_gen("TESTED")

set_property(TARGET run_cloc_tests PROPERTY FOLDER ${CLOC_FOLDER})

if(WIN32)
  add_custom_command(
    TARGET run_cloc_tests
    POST_BUILD
    COMMAND echo deleting offline compiler files and directories...
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/copybuffer_${CLOC_DEFAULT_DEVICE}.bc
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/copybuffer_${CLOC_DEFAULT_DEVICE}.gen
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/copybuffer_${CLOC_DEFAULT_DEVICE}.bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TargetDir}/${CLOC_DEFAULT_DEVICE}/offline_compiler_test
    COMMAND ${run_tests_cmd}
    WORKING_DIRECTORY ${TargetDir}
  )
else()
  add_custom_command(
    TARGET run_cloc_tests
    POST_BUILD
    COMMAND echo deleting offline compiler files and directories...
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/*.bc
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/*.gen
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/*.ll
    COMMAND ${CMAKE_COMMAND} -E remove ${TargetDir}/${CLOC_DEFAULT_DEVICE}/*.bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${TargetDir}/offline_compiler_test"
    COMMAND ${run_tests_cmd}
    WORKING_DIRECTORY ${TargetDir}
  )
endif()
create_project_source_tree(cloc_tests ${IGDRCL_SOURCE_DIR}/runtime ${IGDRCL_SOURCE_DIR}/unit_tests ${IGDRCL_SOURCE_DIR})
