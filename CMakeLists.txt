# Copyright (C) 2014-2015 Bastien Oudot and Romain Guillemot
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

cmake_minimum_required(VERSION 3.0.0)

project(Softbloks CXX)

# set version

set(SOFTBLOKS_VERSION_MAJOR 0)
set(SOFTBLOKS_VERSION_MINOR 1)

set(SOFTBLOKS_BUILD_YEAR    15)
set(SOFTBLOKS_BUILD_MONTH   11)
set(SOFTBLOKS_BUILD_DAY     29)

set(SOFTBLOKS_VERSION
    "${SOFTBLOKS_VERSION_MAJOR}.${SOFTBLOKS_VERSION_MINOR}"
)

# set cmake options

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set OS options

if(WIN32)
    set(SB_INSTALL_ROOT_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_RUNTIME_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}/bin")
    set(SB_INSTALL_LIBRARY_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}/lib")
    set(SB_INSTALL_ARCHIVE_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}/lib")
    set(SB_INSTALL_INCLUDE_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}/include")
    set(SB_INSTALL_SHARE_DESTINATION "sb${SOFTBLOKS_VERSION_MAJOR}/share")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -std=c++11")

    set(SB_INSTALL_ROOT_DESTINATION "share/sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_RUNTIME_DESTINATION "bin/sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_LIBRARY_DESTINATION "lib/sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_ARCHIVE_DESTINATION "lib/sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_INCLUDE_DESTINATION "include/sb${SOFTBLOKS_VERSION_MAJOR}")
    set(SB_INSTALL_SHARE_DESTINATION "share/sb${SOFTBLOKS_VERSION_MAJOR}")
endif()

# install root files

install(
    FILES
    "${PROJECT_SOURCE_DIR}/COPYING"
    "${PROJECT_SOURCE_DIR}/COPYING.LESSER"
    "${PROJECT_SOURCE_DIR}/README.md"
    DESTINATION ${SB_INSTALL_ROOT_DESTINATION}
)

# manage Qt5 targets

function(sb_add_qt5_target _target_type _name)
    if(Qt5_FOUND)
        foreach(_arg ${ARGN})
            string(SUBSTRING ${_arg} 0 5 _arg_prefix)

            if(${_arg_prefix} STREQUAL "Qt5::")
                list(APPEND _qt5_components ${_arg})
            elseif(${_arg_prefix} STREQUAL "rcc::")
                string(SUBSTRING ${_arg} 5 -1 _file_path)
                list(APPEND _resources ${_file_path})
            else()
                list(APPEND _sources ${_arg})
            endif()
        endforeach()

        set(CMAKE_AUTOMOC ON)

        if(_resources)
            qt5_add_resources(_added_resources
                ${_resources}
            )

            list(APPEND _sources ${_added_resources})
        endif()

        if(${_target_type} STREQUAL LIBRARY)
            sb_add_library(${_name}
                ${_sources}
            )
        elseif(${_target_type} STREQUAL EXECUTABLE)
            sb_add_executable(${_name}
                ${_sources}
            )

            if(TARGET Qt5::windeployqt)
                add_custom_command(TARGET ${_name}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
                    COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
                    COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:${_name}>/$<TARGET_FILE_NAME:${_name}>"
                )

                install(
                    DIRECTORY
                    "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
                    DESTINATION ${SB_INSTALL_RUNTIME_DESTINATION}
                )
            endif()
        elseif(${_target_type} STREQUAL MODULE)
            sb_add_module(${_name}
                ${_sources}
            )
        endif()

        target_link_libraries(${_name}
            ${_qt5_components}
        )
    else()
        message(FATAL_ERROR
            "Qt5 is not found."
        )
    endif()
endfunction()

# declare custom add_library functions

function(sb_add_header_only_target _name)
    add_library(${_name} INTERFACE)

    target_include_directories(${_name} INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${SB_INSTALL_INCLUDE_DESTINATION}>
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/"
        DESTINATION ${SB_INSTALL_INCLUDE_DESTINATION}/${_name}
        FILES_MATCHING
        PATTERN *.h
    )
    install(
        TARGETS ${_name}
        EXPORT ${_name}-config
    )
    install(
        EXPORT ${_name}-config
        DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/${_name}
    )
    install(
        FILES "${PROJECT_BINARY_DIR}/share/cmake/config-version.cmake"
        DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/${_name}
        RENAME ${_name}-config-version.cmake
    )

    # make headers displayed in IDEs

    if(MSVC)
        add_custom_target(${_name}.header-only SOURCES ${ARGN})
    endif()
endfunction()

function(sb_add_library _name)
    add_library(${_name} SHARED ${ARGN})

    target_link_libraries(${_name} sb-global)

    string(REGEX REPLACE "-" "_" _c_legal_name ${_name})

    set_target_properties(${_name} PROPERTIES
        DEFINE_SYMBOL ${_c_legal_name}_EXPORTS
        VERSION ${SOFTBLOKS_VERSION}
    )

    target_include_directories(${_name} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${SB_INSTALL_INCLUDE_DESTINATION}>
    )

    install(
        DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/"
        DESTINATION ${SB_INSTALL_INCLUDE_DESTINATION}/${_name}
        FILES_MATCHING
        PATTERN *.h
        PATTERN *-private.h EXCLUDE
    )
    install(
        TARGETS ${_name}
        EXPORT ${_name}-config
        RUNTIME DESTINATION ${SB_INSTALL_RUNTIME_DESTINATION}
        LIBRARY DESTINATION ${SB_INSTALL_LIBRARY_DESTINATION}
        ARCHIVE DESTINATION ${SB_INSTALL_ARCHIVE_DESTINATION}
    )
    install(
        EXPORT ${_name}-config
        DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/${_name}
    )
    install(
        FILES "${PROJECT_BINARY_DIR}/share/cmake/config-version.cmake"
        DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/${_name}
        RENAME ${_name}-config-version.cmake
    )
endfunction()

function(sb_add_qt5_library _name)
    sb_add_qt5_target(
        LIBRARY
        ${_name}
        ${ARGN}
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
        RUNTIME DESTINATION ${SB_INSTALL_RUNTIME_DESTINATION}
    )
endfunction()

function(sb_add_qt5_executable _name)
    sb_add_qt5_target(
        EXECUTABLE
        ${_name}
        ${ARGN}
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
        DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/modules
    )
endfunction()

function(sb_add_qt5_module _name)
    sb_add_qt5_target(
        MODULE
        ${_name}
        ${ARGN}
    )
endfunction()

# find packages

find_package(Qt5 COMPONENTS
    Widgets
    Svg
)

if(Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
    get_target_property(_qt5_qmake_location
        Qt5::qmake IMPORTED_LOCATION
    )

    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

    if(EXISTS ${imported_location})
        add_executable(Qt5::windeployqt IMPORTED)

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()

# add subdirectories

add_subdirectory(share/cmake)
add_subdirectory(share/doc)
add_subdirectory(share/examples)
add_subdirectory(src)

# install system runtime libs

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)

include(InstallRequiredSystemLibraries)

install(
    PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
    DESTINATION ${SB_INSTALL_RUNTIME_DESTINATION}
)
