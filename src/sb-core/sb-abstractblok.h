/*
Copyright (C) 2014 Bastien Oudot

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

#include "sb-abstractobject.h"

namespace sb
{

class SB_CORE_API AbstractBlok : public AbstractObject
{

public:

    AbstractBlok
    (
    );

    virtual
    ~AbstractBlok
    (
    );

    int
    get_input_count
    (
    )
    const;

    int
    get_output_count
    (
    )
    const;

protected:

    class Private;

private:

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTBLOK_H
