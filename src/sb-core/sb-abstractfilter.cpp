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

AbstractFilter::AbstractFilter
(
)
{
    d_ptr = new Private(this);
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
AbstractFilter::set_input_count
(
    int _minimum,
    int _maximum
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

void
AbstractFilter::set_output_count
(
    int _minimum,
    int _maximum
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
AbstractFilter::set_step_range_converter
(
    size_t _output,
    const StepRangeConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->step_range_converters.at(_output) = _value;
}

void
AbstractFilter::set_defined_steps_converter
(
    size_t _output,
    const StepListConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->defined_steps_converters.at(_output) = _value;
}

void
AbstractFilter::set_wanted_steps_converter
(
    size_t _input,
    const StepListConverter& _value
)
{
    AbstractBlok::Private::from(
        this
    )->wanted_steps_converters.at(_input) = _value;
}

AbstractFilter::Private::Private
(
    AbstractFilter* _q
):
    q_ptr(_q)
{
}
