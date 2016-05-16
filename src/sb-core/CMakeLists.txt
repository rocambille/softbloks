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

sb_add_library(sb-core
    sb-abstractblok.cpp
    sb-abstractblok.h
    sb-abstractblok-private.h
    sb-abstractdata.cpp
    sb-abstractdata.h
    sb-abstractdata-private.h
    sb-abstractexecutive.cpp
    sb-abstractexecutive.h
    sb-abstractexecutive-private.h
    sb-abstractfilter.cpp
    sb-abstractfilter.h
    sb-abstractfilter-private.h
    sb-abstractobject.cpp
    sb-abstractobject.h
    sb-abstractobject-private.h
    sb-abstractsink.cpp
    sb-abstractsink.h
    sb-abstractsink-private.h
    sb-abstractsoft.cpp
    sb-abstractsoft.h
    sb-abstractsoft-private.h
    sb-abstractsource.cpp
    sb-abstractsource.h
    sb-abstractsource-private.h
    sb-blokformat.h
    sb-core.h
    sb-coredefine.h
    sb-data.h
    sb-executive.cpp
    sb-executive.h
    sb-executive-private.h
    sb-objectformat.h
    sb-property.h
    sb-propertyformat.h
)
