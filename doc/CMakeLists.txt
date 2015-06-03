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

find_package(Doxygen)

option(BUILD_DOCUMENTATION
    "build HTML documentation with Doxygen"
    ${DOXYGEN_FOUND}
)

if(BUILD_DOCUMENTATION)
    if(DOXYGEN_FOUND)
        configure_file(
            softbloks.doxyfile.in
            "${CMAKE_CURRENT_BINARY_DIR}/softbloks.doxyfile"
            @ONLY
        )

        add_custom_target(doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/softbloks.doxyfile"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )

        install(
            DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/
            DESTINATION share${SB_INSTALL_SUBDIRECTORY}/doc
        )
    else()
        message(FATAL_ERROR
            "Doxygen is required to build documentation."
        )
    endif()
endif()
