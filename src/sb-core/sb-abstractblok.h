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
#include <sb-core/sb-blokformat.h>

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
    static
    ObjectFormatSequence
    get_inputs_format
    (
    )
    {
        return { };
    }

    static
    ObjectFormatSequence
    get_outputs_format
    (
    )
    {
        return { };
    }

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

private:

    /// \cond INTERNAL
    static
    void
    init
    (
        AbstractBlok* this_,
        const ObjectFormatSequence& inputs_format_,
        const ObjectFormatSequence& outputs_format_
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
    AbstractBlok::get_properties_format()
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

template<typename T>
inline
bool
register_blok
(
)
{
    ObjectFactory factory = (
        []
        (
        )
        {
            UniqueBlok instance = UniqueBlok(
                new T,
                []
                (
                    AbstractObject* ptr_
                )
                {
                    delete ptr_;

                    AbstractObject::forget(ptr_);
                }
            );

            AbstractBlok::init(
                instance.get(),
                T::get_inputs_format(),
                T::get_outputs_format()
            );

            AbstractObject::init(
                instance.get(),
                T::get_type_names(),
                T::get_properties()
            );

            return static_move_cast<AbstractObject>(
                std::move(instance)
            );
        }
    );

    return AbstractObject::register_object(
        {
            T::get_type_names(),
            T::get_properties_format()
        },
        factory
    );
}

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

#define SB_INPUTS_FORMAT(...)\
    public:\
        static\
        sb::ObjectFormatSequence\
        get_inputs_format\
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
                "Inputs format declared on a type not derived from "\
                "sb::AbstractFilter nor sb::AbstractSink"\
            );\
            return sb::make_object_format_sequence(__VA_ARGS__);\
        }

#define SB_OUTPUTS_FORMAT(...)\
    public:\
        static\
        sb::ObjectFormatSequence\
        get_outputs_format\
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
                "Outputs format declared on a type not derived from "\
                "sb::AbstractFilter nor sb::AbstractSource"\
            );\
            return sb::make_object_format_sequence(__VA_ARGS__);\
        }

#endif // SB_ABSTRACTBLOK_H
