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

if(NOT softbloks_FIND_COMPONENTS)
    set(softbloks_NOT_FOUND_MESSAGE "The softbloks package requires at least one component")
    set(softbloks_FOUND FALSE)
    return()
endif()

set(_sb_FIND_REQUIRED)

if(softbloks_FIND_REQUIRED)
    set(_sb_FIND_REQUIRED REQUIRED)
endif()

set(_sb_FIND_QUIETLY)

if(softbloks_FIND_QUIETLY)
    set(_sb_FIND_QUIETLY QUIET)
endif()

get_filename_component(_sb_cmake_prefix "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

set(_sb_NOTFOUND_MESSAGE)

foreach(module ${softbloks_FIND_COMPONENTS})
    find_package(${module}
        ${_sb_FIND_PARTS_QUIET}
        ${_sb_FIND_PARTS_REQUIRED}
        PATHS "${_sb_cmake_prefix}/${module}" NO_DEFAULT_PATH
    )

    if(NOT TARGET ${module})
        if(softbloks_FIND_REQUIRED_${module})
            set(_sb_NOTFOUND_MESSAGE
                "${_sb_NOTFOUND_MESSAGE}"
                "Failed to find softbloks component ${module}"
            )
        elseif(NOT softbloks_FIND_QUIETLY)
            message(WARNING "Failed to find softbloks component ${module}")
        endif()
    endif()
endforeach()

if(_sb_NOTFOUND_MESSAGE)
    set(softbloks_NOT_FOUND_MESSAGE "${_sb_NOTFOUND_MESSAGE}")
    set(softbloks_FOUND FALSE)
endif()
