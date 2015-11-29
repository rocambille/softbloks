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

find_package(GTest)

option(BUILD_TESTING
    "build testing"
    ${GTEST_FOUND}
)

function(sb_add_test _name)
    sb_add_executable(${_name} ${ARGN})

    target_link_libraries(${_name} ${GTEST_BOTH_LIBRARIES})
endfunction()

if(BUILD_TESTING)
    if(GTEST_FOUND)
        include_directories(${GTEST_INCLUDE_DIRS})

        sb_add_test(sb-core-test
            sb-abstractobject-test.h
            sb-coredefine-test.h
            sb-core-test.cpp
            sb-fixtures.h
        )
    else()
        message(FATAL_ERROR
            "GTest is required to build tests."
        )
    endif()
endif()
