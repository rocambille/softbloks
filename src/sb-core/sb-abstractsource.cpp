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
#include "sb-abstractsource.h"

#include "sb-abstractsource-private.h"

#include "sb-abstractblok-private.h"
#include "sb-abstractdata-private.h"

using namespace sb;

AbstractSource::~AbstractSource
(
)
{
    delete d_ptr;
}

SharedDataSet
AbstractSource::get_output
(
    size_t _index
)
const
{
    return AbstractBlok::Private::from(
        this
    )->outputs.at(_index);
}

AbstractSource::AbstractSource
(
)
{
    d_ptr = new Private(this);
}

void
AbstractSource::set_output_count
(
    size_t _value
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_count(
        _value
    );
}

void
AbstractSource::set_output_count
(
    size_t _minimum,
    size_t _maximum
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_count(
        _minimum,
        _maximum
    );
}

void
AbstractSource::set_output_format
(
    size_t _index,
    const ObjectInformation& _format
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_format(
        _index,
        _format
    );
}

void
AbstractSource::set_index_range
(
    size_t _output,
    const IndexRange& _value
)
{
    DataSet::Private::from(
        this->get_output(
            _output
        ).get()
    )->set_index_range(
        _value
    );
}

void
AbstractSource::set_defined_indices
(
    size_t _output,
    const IndexCollection& _value
)
{
    DataSet::Private::from(
        this->get_output(
            _output
        ).get()
    )->set_defined_indices(
        _value
    );
}

AbstractSource::Private::Private
(
    AbstractSource* _q
):
    q_ptr(_q)
{
}
