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
#ifndef SB_ABSTRACTSOURCE_H
#define SB_ABSTRACTSOURCE_H

#include <sb-core/sb-abstractblok.h>

namespace sb
{

class SB_CORE_API AbstractSource : public AbstractBlok
{

    SB_SELF(sb::AbstractSource)

    SB_NAME("sb.AbstractSource")

public:

    class Private;

    /// Constructs a source.
    AbstractSource
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractSource
    (
    );

    SharedData
    get_output
    (
        Index index_ = 0
    )
    const;

private:

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

/// Alias for a managed source uniquely owned.
using UniqueSource = Unique<AbstractSource>;

/// Alias for create_unique<AbstractSource>().
static
UniqueSource
(&create_unique_source)
(
    const std::string& name_
) = create_unique<AbstractSource>;

}

#endif // SB_ABSTRACTSOURCE_H
