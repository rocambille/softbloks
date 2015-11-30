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
#ifndef SB_ABSTRACTDATA_H
#define SB_ABSTRACTDATA_H

#include <sb-core/sb-abstractobject.h>

namespace sb
{

class SB_CORE_API AbstractData : public AbstractObject
{

    SB_DECLARE_OBJECT(AbstractData, "sb::AbstractData")

public:

    class Private;

    virtual
    ~AbstractData
    (
    );

private:

    static
    void
    construct
    (
        AbstractData* this_
    );

private:

    Private*
    d_ptr;

};

using SharedData = Shared<AbstractData>;

static
SharedData
(&create_shared_data)
(
    const std::string& name_
) = create_shared<AbstractData>;

}

#endif // SB_ABSTRACTDATA_H
