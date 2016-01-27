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

SB_CLASS(SB_CORE_API, AbstractExecutive, "sb.AbstractExecutive", AbstractObject)
{

public:

    class Private;

    AbstractExecutive
    (
    );

    virtual
    ~AbstractExecutive
    (
    );

    virtual
    void
    on_input_pushed
    (
        size_t index_
    )
    = 0;

    virtual
    void
    on_output_pulled
    (
        size_t index_
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

    Private*
    d_ptr;

};

using UniqueExecutive = Unique<AbstractExecutive>;

static
UniqueExecutive
(&create_unique_executive)
(
    const std::string& name_
) = create_unique<AbstractExecutive>;

}

#endif // SB_ABSTRACTEXECUTIVE_H
