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

#include <sb-core/sb-abstractexecutive.h>
#include <sb-core/sb-dataset.h>

namespace sb
{

/// \brief The AbstractBlok class is the base class for dataflow objects.
///
/// A dataflow object can be either a source, a filter or a sink.
class SB_CORE_API AbstractBlok : public sb::AbstractObject
{

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

    sb::Size
    get_minimum_input_count
    (
    )
    const;

    sb::Size
    get_maximum_input_count
    (
    )
    const;

    sb::Size
    get_input_count
    (
    )
    const;

    sb::Size
    get_minimum_output_count
    (
    )
    const;

    sb::Size
    get_maximum_output_count
    (
    )
    const;

    sb::Size
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
        sb::Index index_
    );

    void
    push_output
    (
        sb::Index index_
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
    Private*
    d_ptr;
    /// \endcond

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

}

/// \cond INTERNAL
SB_DECLARE_CLASS(
    sb::AbstractBlok,
    "sb.AbstractBlok",
    sb::AbstractObject
)
/// \endcond

#endif // SB_ABSTRACTBLOK_H
