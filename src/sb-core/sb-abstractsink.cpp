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
#include "sb-abstractsink.h"

#include "sb-abstractsink-private.h"

#include "sb-abstractblok-private.h"

using namespace sb;

AbstractSink::Private::Private
(
    AbstractSink* _q
):
    q_ptr(_q)
{
}

AbstractSink::AbstractSink
(
)
{
    d_ptr = new Private(this);
}

AbstractSink::~AbstractSink
(
)
{
    delete d_ptr;
}

void
AbstractSink::set_input_count
(
    size_t _value
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_count(
        _value
    );
}

void
AbstractSink::set_input_count
(
    size_t _minimum,
    size_t _maximum
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_count(
        _minimum,
        _maximum
    );
}
