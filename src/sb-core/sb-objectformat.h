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
#ifndef SB_OBJECTFORMAT_H
#define SB_OBJECTFORMAT_H

#include <sb-core/sb-propertyformat.h>

#include <algorithm>

namespace sb
{

/// \brief The ObjectFormat structure describes the format of an
/// AbstractObject.
///
/// The format includes the type name of an object, the type names of its base
/// classes and a list of properties.
struct ObjectFormat
{

    /// This attribute holds a list of type names.
    ///
    /// The first one in the the list describes the type of the object. Next
    /// in the list are the types of the object's base class(es), the base(s)
    /// of the base(s) and so on, listing the complete inheritance tree of the
    /// object.
    ///
    /// \sa SB_NAME().
    StringSequence
    type_names;

    /// This attribute holds a list of properties.
    ///
    /// \sa SB_PROPERTIES().
    PropertyFormatSequence
    properties_format;

    inline
    bool
    includes
    (
        const ObjectFormat& other_
    )
    const
    {
        // check other_ is not "empty":
        // including an empty format always returns false

        bool is_included = (
            other_.type_names.size() > 0
        );

        // check type names

        if(is_included)
        {
            StringSequence this_type_names =
                this->type_names;
            std::sort(
                this_type_names.begin(),
                this_type_names.end()
            );

            StringSequence other_type_names =
                other_.type_names;
            std::sort(
                other_type_names.begin(),
                other_type_names.end()
            );

            is_included = std::includes(
                this_type_names.begin(),
                this_type_names.end(),
                other_type_names.begin(),
                other_type_names.end()
            );
        }

        // check properties formats

        if(is_included)
        {
            PropertyFormatSequence this_properties_format =
                this->properties_format;
            std::sort(
                this_properties_format.begin(),
                this_properties_format.end()
            );

            PropertyFormatSequence other_properties_format =
                other_.properties_format;
            std::sort(
                other_properties_format.begin(),
                other_properties_format.end()
            );

            is_included = std::includes(
                this_properties_format.begin(),
                this_properties_format.end(),
                other_properties_format.begin(),
                other_properties_format.end()
            );
        }

        // return the result

        return is_included;
    }

};

/// Constant value representing the format of an invalid object (not
/// inheriting AbstractObject).
const ObjectFormat
UNDEFINED_OBJECT_FORMAT = { { } };

inline
bool
operator==
(
    const ObjectFormat& left_,
    const ObjectFormat& right_
)
{
    return (
        left_.type_names == right_.type_names
    ) && (
        left_.properties_format == right_.properties_format
    );
}

inline
ObjectFormat&
operator<<
(
    ObjectFormat& format_,
    const std::string& type_name_
)
{
    format_.type_names.push_back(type_name_);
    return format_;
}

inline
ObjectFormat&
operator<<
(
    ObjectFormat&& format_,
    const std::string& type_name_
)
{
    return format_ << type_name_;
}

inline
ObjectFormat&
operator<<
(
    ObjectFormat& format_,
    const PropertyFormat& property_format_
)
{
    format_.properties_format.push_back(property_format_);
    return format_;
}

inline
ObjectFormat&
operator<<
(
    ObjectFormat&& format_,
    const PropertyFormat& property_format_
)
{
    return format_ << property_format_;
}

using ObjectFormatSequence = std::vector<ObjectFormat>;

/// \cond INTERNAL
template<typename... Types>
inline
ObjectFormatSequence
make_object_format_sequence
(
    Types&&... args_
)
{
    return { args_... };
}

template<>
inline
ObjectFormatSequence
make_object_format_sequence
(
    ObjectFormatSequence&& value_
)
{
    return value_;
}
/// \endcond

}

#endif // SB_OBJECTFORMAT_H
