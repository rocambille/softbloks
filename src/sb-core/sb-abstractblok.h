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
#ifndef SB_ABSTRACTBLOK_H
#define SB_ABSTRACTBLOK_H

#include <sb-core/sb-abstractobject.h>

#include <sb-core/sb-abstractdata.h>
#include <sb-core/sb-abstractexecutive.h>
#include <sb-core/sb-data.h>

#include <type_traits>

namespace sb
{

/// \brief The AbstractBlok class is the base class for dataflow objects.
///
/// A dataflow object can be either a source, a filter or a sink.
class SB_CORE_API AbstractBlok : public AbstractObject
{

    SB_SELF(sb::AbstractBlok)

    SB_NAME("sb.AbstractBlok")

public:

    class Private;

    /// Constructs a blok.
    AbstractBlok
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractBlok
    (
    );

    Size
    get_input_count
    (
    )
    const;

    Size
    get_output_count
    (
    )
    const;

    void
    use_executive
    (
        const std::string& name_
    );

    void
    pull_input
    (
        Index index_ = 0
    );

    void
    push_output
    (
        Index index_ = 0
    );

    virtual
    void
    process
    (
    )
    {
    }

    static
    ObjectFormatSequence
    get_inputs_formats
    (
    )
    {
        return { };
    }

    static
    StringSequence
    get_outputs_type_names
    (
    )
    {
        return { };
    }

    static
    ObjectFormatSequence
    get_outputs_formats
    (
    )
    {
        return { };
    }

    /// \cond INTERNAL
    template<typename T>
    static
    void
    on_creation
    (
        AbstractBlok* this_
    )
    {
        AbstractBlok::init(
            this_,
            T::get_inputs_formats(),
            T::get_outputs_type_names()
        );

        AbstractObject::on_creation<T>(this_);
    }
    /// \endcond

private:

    /// \cond INTERNAL
    static
    void
    init
    (
        AbstractBlok* this_,
        const ObjectFormatSequence& inputs_formats_,
        const StringSequence& outputs_type_names_
    );
    /// \endcond

private:

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

/// Constant value representing the format of a valid blok (inheriting
/// AbstractBlok).
const ObjectFormat
ANY_BLOK_FORMAT = {
    AbstractBlok::get_type_names(),
    AbstractBlok::get_properties()
};

/// Alias for a managed blok uniquely owned.
using UniqueBlok = Unique<AbstractBlok>;

/// Alias for create_unique<AbstractBlok>().
static
UniqueBlok
(&create_unique_blok)
(
    const std::string& name_
) = create_unique<AbstractBlok>;

SB_CORE_API
bool
connect
(
    AbstractBlok* left_,
    Index left_index_,
    AbstractBlok* right_,
    Index right_index_
);

template<typename T, typename U>
inline
bool
connect(
    const T& left_,
    Index left_index_,
    U& right_,
    Index right_index_
)
{
    return connect(
        left_.get(),
        left_index_,
        right_.get(),
        right_index_
    );
}

inline
bool
connect
(
    AbstractBlok* left_,
    AbstractBlok* right_
)
{
    return connect(left_, 0, right_, 0);
}

template<typename T, typename U>
inline
bool
connect(
    const T& left_,
    U& right_
)
{
    return connect(left_.get(), 0, right_.get(), 0);
}

}

#define SB_INPUTS_FORMATS(...)\
    public:\
        static\
        sb::ObjectFormatSequence\
        get_inputs_formats\
        (\
        )\
        {\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_same<sb::AbstractFilter, Self>::value ||\
                    std::is_base_of<sb::AbstractFilter, Self>::value ||\
                    std::is_same<sb::AbstractSink, Self>::value ||\
                    std::is_base_of<sb::AbstractSink, Self>::value\
                ),\
                "inputs formats declared on a type not derived from "\
                "sb::AbstractFilter nor sb::AbstractSink"\
            );\
            return sb::make_object_format_sequence(__VA_ARGS__);\
        }

#define SB_INPUTS_TYPES(...)\
    public:\
        static\
        sb::ObjectFormatSequence\
        get_inputs_formats\
        (\
        )\
        {\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_same<sb::AbstractFilter, Self>::value ||\
                    std::is_base_of<sb::AbstractFilter, Self>::value ||\
                    std::is_same<sb::AbstractSink, Self>::value ||\
                    std::is_base_of<sb::AbstractSink, Self>::value\
                ),\
                "inputs formats declared on a type not derived from "\
                "sb::AbstractFilter nor sb::AbstractSink"\
            );\
            return sb::make_object_format_sequence<__VA_ARGS__>();\
        }

#define SB_OUTPUTS_TYPES(...)\
    public:\
        static\
        sb::StringSequence\
        get_outputs_type_names\
        (\
        )\
        {\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_same<sb::AbstractFilter, Self>::value ||\
                    std::is_base_of<sb::AbstractFilter, Self>::value ||\
                    std::is_same<sb::AbstractSource, Self>::value ||\
                    std::is_base_of<sb::AbstractSource, Self>::value\
                ),\
                "outputs type names declared on a type not derived from "\
                "sb::AbstractFilter nor sb::AbstractSource"\
            );\
            return sb::make_type_name_sequence<__VA_ARGS__>();\
        }\
        static\
        sb::ObjectFormatSequence\
        get_outputs_formats\
        (\
        )\
        {\
            return sb::make_object_format_sequence<__VA_ARGS__>();\
        }

#endif // SB_ABSTRACTBLOK_H
