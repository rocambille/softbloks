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

option(BUILD_TESTING
    "build testing"
    OFF
)

function(sb_add_test _name)
    sb_add_executable(${_name} ${ARGN})

    target_link_libraries(${_name} gtest_main)
endfunction()

if(BUILD_TESTING)
    # add googletest to the build tree

    set(BUILD_GTEST
        ON
        CACHE
        BOOL
        "Builds the googletest subproject"
    )
    set(BUILD_GMOCK
        OFF
        CACHE
        BOOL
        "Builds the googlemock subproject"
    )
    set(gtest_force_shared_crt
        ON
        CACHE
        BOOL
        "Use shared (DLL) run-time lib even when Google Test is built as static lib."
    )

    add_subdirectory(
        "${PROJECT_SOURCE_DIR}/3rdparty/googletest"
        "${PROJECT_BINARY_DIR}/3rdparty/googletest"
        EXCLUDE_FROM_ALL
    )

    # add the tests

    sb_add_test(sb-core-test
        sb-abstractobject-test.h
        sb-core-test.cpp
        sb-fixtures.h
        sb-objectformat-test.h
        sb-propertyformat-test.h
        sb-static_move_cast-test.h
    )
endif()
