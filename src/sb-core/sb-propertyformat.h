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
#ifndef SB_PROPERTYFORMAT_H
#define SB_PROPERTYFORMAT_H

#include <sb-core/sb-coredefine.h>

#include <typeindex>
#include <vector>

namespace sb
{

/// \brief The PropertyFormat structure represents the format of an
/// AbstractObject's property.
///
/// The format includes the type and the access rights of the property.
///
/// \sa \ref property-system "Softbloks's property system".
struct PropertyFormat
{

    std::type_index
    type;

    std::string
    name;

    AccessRights
    access_rights;

};

/// Compares the property formats \a left_ and \a right_.
///
/// The comparison returns \b true if the described properties have the same
/// types and the same access rights; it returns \b false otherwise.
inline
bool
operator==
(
    const PropertyFormat& left_,
    const PropertyFormat& right_
)
{
    return (
        left_.type == right_.type
    ) && (
        left_.name == right_.name
    ) && (
        left_.access_rights == right_.access_rights
    );
}

inline
bool
operator<
(
    const PropertyFormat& left_,
    const PropertyFormat& right_
)
{
    return left_.name < right_.name;
}

template<typename T>
PropertyFormat
make_property_format
(
    const std::string& name_,
    AccessRights access_rights_
)
{
    return{
        typeid(T),
        name_,
        access_rights_
    };
}

using PropertyFormatSequence = std::vector<PropertyFormat>;

}

#endif // SB_PROPERTYFORMAT_H
