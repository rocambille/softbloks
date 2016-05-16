/*
Copyright (C) 2014-2015 Bastien Oudot and Romain Guillemot

This file is part of Softbloks.
Softbloks is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Softbloks is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Softbloks.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SB_BLOKFORMAT_H
#define SB_BLOKFORMAT_H

#include <sb-core/sb-objectformat.h>

namespace sb
{

/// \brief The ObjectFormat structure describes the format of an
/// AbstractObject.
///
/// The format includes the type name of an object, the type names of its base
/// classes and a list of properties.
struct BlokFormat : ObjectFormat
{

    ObjectFormatSequence
    inputs_format;

    ObjectFormatSequence
    outputs_format;

};

}

#endif // SB_BLOKFORMAT_H
