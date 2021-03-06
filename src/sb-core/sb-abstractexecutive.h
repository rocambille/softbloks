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
#ifndef SB_ABSTRACTEXECUTIVE_H
#define SB_ABSTRACTEXECUTIVE_H

#include <sb-core/sb-abstractobject.h>

namespace sb
{

class AbstractBlok;

class SB_CORE_API AbstractExecutive : public AbstractObject
{

    SB_SELF(sb::AbstractExecutive)

    SB_NAME("sb.AbstractExecutive")

public:

    class Private;

    /// Constructs an executive.
    AbstractExecutive
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractExecutive
    (
    );

    virtual
    void
    on_input_pushed
    (
        Index index_
    )
    = 0;

    virtual
    void
    on_output_pulled
    (
        Index index_
    )
    = 0;

protected:

    AbstractBlok*
    get_blok
    (
    )
    const;

    void
    execute
    (
    );

private:

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

/// Alias for a managed executive uniquely owned.
using UniqueExecutive = Unique<AbstractExecutive>;

/// Alias for create_unique<AbstractExecutive>().
static
UniqueExecutive
(&create_unique_executive)
(
    const std::string& name_
) = create_unique<AbstractExecutive>;

}

#endif // SB_ABSTRACTEXECUTIVE_H
