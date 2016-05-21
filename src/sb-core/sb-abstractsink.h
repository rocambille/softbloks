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
#ifndef SB_ABSTRACTSINK_H
#define SB_ABSTRACTSINK_H

#include <sb-core/sb-abstractblok.h>

namespace sb
{

class SB_CORE_API AbstractSink : public AbstractBlok
{

    SB_SELF(sb::AbstractSink)

    SB_NAME("sb.AbstractSink")

public:

    class Private;

    /// Constructs a sink.
    AbstractSink
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractSink
    (
    );

    SharedData
    lock_input
    (
        Index index_ = 0
    )
    const;

    bool
    set_input
    (
        Index index_,
        const SharedData& value_
    );

private:

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

/// Alias for a managed sink uniquely owned.
using UniqueSink = Unique<AbstractSink>;

/// Alias for create_unique<AbstractSink>().
static
UniqueSink
(&create_unique_sink)
(
    const std::string& name_
) = create_unique<AbstractSink>;

}

#endif // SB_ABSTRACTSINK_H
