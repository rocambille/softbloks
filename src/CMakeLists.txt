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

# add global module

add_subdirectory(sb-global)

# add core module

add_subdirectory(sb-core)

# add software

option(BUILD_SOFTWARE
    "build software"
    ${Qt5_FOUND}
)

if(BUILD_SOFTWARE)
    if(Qt5_FOUND)
        add_subdirectory(software)
    else()
        message(FATAL_ERROR
            "Qt5 is not found."
        )
    endif()
endif()
