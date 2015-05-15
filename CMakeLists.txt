# Copyright (C) 2014 Bastien Oudot
#
# This file is part of Softbloks.
# Softbloks is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Softbloks is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Softbloks.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8.11)

project(Softbloks C CXX)

# set version

set(SOFTBLOKS_VERSION_MAJOR 0)
set(SOFTBLOKS_VERSION_MINOR 0)

set(SOFTBLOKS_BUILD_YEAR    14)
set(SOFTBLOKS_BUILD_MONTH   12)
set(SOFTBLOKS_BUILD_DAY     30)

set(SOFTBLOKS_VERSION
    "${SOFTBLOKS_VERSION_MAJOR}.${SOFTBLOKS_VERSION_MINOR}"
)

# set cmake options

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set OS options

if(MSVC)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")
endif()

# install root files

install(
    FILES
    "${PROJECT_SOURCE_DIR}/COPYING"
    "${PROJECT_SOURCE_DIR}/COPYING.LESSER"
    "${PROJECT_SOURCE_DIR}/README.md"
    DESTINATION .
)

# declare custom add_library functions

function(sb_add_library _name)
    add_library(${_name} SHARED ${ARGN})

    string(REGEX REPLACE "-" "_" _c_legal_name ${_name})

    set_target_properties(${_name} PROPERTIES
        DEFINE_SYMBOL ${_c_legal_name}_EXPORTS
        VERSION ${SOFTBLOKS_VERSION}
    )

    target_include_directories(${_name} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/sb-global>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:include/sb-global>
        $<INSTALL_INTERFACE:include/${_name}>
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/"
        DESTINATION include/${_name}
        FILES_MATCHING
        PATTERN *.h
        PATTERN *-private.h EXCLUDE
    )
    install(
        TARGETS ${_name}
        EXPORT ${_name}-config
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    install(
        EXPORT ${_name}-config
        DESTINATION lib/cmake/${_name}
    )
endfunction()

# declare custom add_executable functions

function(sb_add_executable _name)
    add_executable(${_name} ${ARGN})

    target_link_libraries(${_name} sb-core)

    set_target_properties(${_name} PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
        VERSION ${SOFTBLOKS_VERSION}
    )

    install(
        TARGETS ${_name}
        RUNTIME DESTINATION bin
    )
endfunction()

# declare sb_add_module functions

function(sb_add_module _name)
    add_library(${_name} MODULE ${ARGN})

    target_link_libraries(${_name} sb-core)

    set_target_properties(${_name} PROPERTIES
        VERSION ${SOFTBLOKS_VERSION}
    )

    install(
        TARGETS ${_name}
        DESTINATION lib/modules
    )
endfunction()

function(sb_add_qt5_module _name)
    if(Qt5_FOUND)
        foreach(_arg ${ARGN})
            string(SUBSTRING ${_arg} 0 5 _arg_prefix)

            if(${_arg_prefix} STREQUAL "Qt5::")
                list(APPEND _qt5_components ${_arg})
            else()
                list(APPEND _sources ${_arg})
            endif()
        endforeach()

        sb_add_module(${_name}
            ${_sources}
        )

        target_link_libraries(${_name}
            ${_qt5_components}
        )
    else()
        message(FATAL_ERROR
            "Qt5 is not found."
        )
    endif()
endfunction()

# find packages

find_package(Qt5 COMPONENTS
    Widgets
    Svg
)

# add subdirectories

add_subdirectory(cmake)
add_subdirectory(doc)
add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(tests)
