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

sb_add_executable(softrun
    sr-directory.cpp
    sr-directory.h
    sr-directory-private.h
    sr-library.cpp
    sr-library.h
    sr-library-private.h
    sr-main.cpp
)

if(UNIX)
    target_link_libraries(softrun
        ${CMAKE_DL_LIBS}
    )
endif()
