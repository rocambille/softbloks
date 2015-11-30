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

install(
    FILES softbloks-config.cmake
    DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/softbloks
)

configure_file(config-version.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/config-version.cmake"
    @ONLY
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/config-version.cmake"
    DESTINATION ${SB_INSTALL_SHARE_DESTINATION}/cmake/softbloks
    RENAME softbloks-config-version.cmake
)
