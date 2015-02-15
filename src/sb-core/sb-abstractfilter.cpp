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

AbstractFilter::~AbstractFilter
(
)
{
    delete d_ptr;
}

SharedDataSet
AbstractFilter::get_input
(
    size_t _index
)
const
{
    return AbstractBlok::Private::from(
        this
    )->inputs.at(_index);
}

bool
AbstractFilter::set_input
(
    size_t _index,
    const SharedDataSet& _value
)
{
    return AbstractBlok::Private::from(
        this
    )->set_input(
        _index,
        _value
    );
}

SharedDataSet
AbstractFilter::get_output
(
    size_t _index
)
const
{
    return AbstractBlok::Private::from(
        this
    )->outputs.at(_index);
}

void
AbstractFilter::set_input_count
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
AbstractFilter::set_input_count
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
AbstractFilter::set_input_format
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
AbstractFilter::set_output_count
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
AbstractFilter::set_output_count
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
AbstractFilter::set_output_format
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
AbstractFilter::set_index_range_converter
(
    size_t _output,
    const IndexRangeConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->index_range_converters.at(_output) = _value;
}

void
AbstractFilter::set_defined_indices_converter
(
    size_t _output,
    const IndexCollectionConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->defined_indices_converters.at(
        _output
    ) = (
        _value
    );
}

void
AbstractFilter::set_wanted_indices_converter
(
    size_t _input,
    const IndexCollectionConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->wanted_indices_converters.at(
        _input
    ) = (
        _value
    );
}

void
AbstractFilter::construct
(
    AbstractFilter* _this
)
{
    _this->d_ptr = new Private(_this);
}

AbstractFilter::Private::Private
(
    AbstractFilter* _q
):
    q_ptr(_q)
{
}
