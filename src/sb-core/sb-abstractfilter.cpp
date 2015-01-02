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
#include "sb-abstractfilter.h"

#include "sb-abstractfilter-private.h"

#include "sb-abstractblok-private.h"

using namespace sb;

AbstractFilter::Private::Private
(
    AbstractFilter* _q
):
    q_ptr(_q)
{
}

AbstractFilter::AbstractFilter
(
)
{
    d_ptr = new Private(this);
}

AbstractFilter::~AbstractFilter
(
)
{
    delete d_ptr;
}

void
AbstractFilter::set_input_count
(
    int _value
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_count(
        _value
    );
}

void
AbstractFilter::set_output_count
(
    int _value
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_count(
        _value
    );
}
