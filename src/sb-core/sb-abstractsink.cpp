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
#include "sb-abstractdata-private.h"

using namespace sb;

AbstractSink::~AbstractSink
(
)
{
    delete d_ptr;
}

SharedDataSet
AbstractSink::get_input
(
    size_t _index
)
const
{
    return AbstractBlok::Private::from(
        this
    )->inputs.at(_index);
}

void
AbstractSink::set_input
(
    size_t _index,
    const SharedDataSet& _value
)
{
    AbstractBlok::Private::from(
        this
    )->set_input(
        _index,
        _value
    );
}

void
AbstractSink::set_wanted_indices
(
    size_t _input,
    const IndexCollection& _value
)
{
    DataSet::Private::from(
        this->get_input(
            _input
        ).get()
    )->set_wanted_indices(
        _value
    );
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

void
AbstractSink::set_input_format
(
    size_t _index,
    const ObjectInformation& _format
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_format(
        _index,
        _format
    );
}

void
AbstractSink::construct
(
    AbstractSink* _this
)
{
    _this->d_ptr = new Private(_this);
}

AbstractSink::Private::Private
(
    AbstractSink* _q
):
    q_ptr(_q)
{
}
