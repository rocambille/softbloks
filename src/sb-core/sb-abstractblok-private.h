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
#ifndef SB_ABSTRACTBLOK_PRIVATE_H
#define SB_ABSTRACTBLOK_PRIVATE_H

#include "sb-abstractblok.h"

namespace sb
{

class AbstractBlok::Private
{

public:

    AbstractBlok::Private
    (
        AbstractBlok* _q
    );

    void
    set_input_count
    (
        int _value
    );

    void
    set_output_count
    (
        int _value
    );

    static
    AbstractBlok::Private*
    from
    (
        AbstractBlok* _q
    );

public:

    AbstractBlok*
    q_ptr;

    int
    input_count;

    int
    output_count;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
