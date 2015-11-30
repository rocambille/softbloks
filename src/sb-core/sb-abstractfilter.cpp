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
#include <sb-core/sb-abstractfilter.h>

#include <sb-core/sb-abstractfilter-private.h>

#include <sb-core/sb-abstractblok-private.h>

using namespace sb;

AbstractFilter::~AbstractFilter
(
)
{
    delete d_ptr;
}

SharedDataSet
AbstractFilter::lock_input
(
    size_t index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->inputs.at(index_).lock();
}

bool
AbstractFilter::set_input
(
    size_t index_,
    const SharedDataSet& value_
)
{
    return AbstractBlok::Private::from(
        this
    )->set_input(
        index_,
        value_
    );
}

SharedDataSet
AbstractFilter::get_output
(
    size_t index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->outputs.at(index_);
}

void
AbstractFilter::set_input_count
(
    size_t value_
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_count(
        value_
    );
}

void
AbstractFilter::set_input_count
(
    size_t minimum_,
    size_t maximum_
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_count(
        minimum_,
        maximum_
    );
}

void
AbstractFilter::set_input_format
(
    size_t index_,
    const ObjectFormat& format_
)
{
    AbstractBlok::Private::from(
        this
    )->set_input_format(
        index_,
        format_
    );
}

void
AbstractFilter::set_output_count
(
    size_t value_
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_count(
        value_
    );
}

void
AbstractFilter::set_output_count
(
    size_t minimum_,
    size_t maximum_
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_count(
        minimum_,
        maximum_
    );
}

void
AbstractFilter::set_output_format
(
    size_t index_,
    const ObjectFormat& format_
)
{
    AbstractBlok::Private::from(
        this
    )->set_output_format(
        index_,
        format_
    );
}

void
AbstractFilter::set_index_range_converter
(
    size_t output_,
    const IndexRangeConverter& value_
)
{
    AbstractBlok::Private::from(
        this
    )->index_range_converters.at(output_) = value_;
}

void
AbstractFilter::set_defined_indices_converter
(
    size_t output_,
    const IndexCollectionConverter& value_
)
{
    AbstractBlok::Private::from(
        this
    )->defined_indices_converters.at(
        output_
    ) = (
        value_
    );
}

void
AbstractFilter::set_wanted_indices_converter
(
    size_t input_,
    const IndexCollectionConverter& value_
)
{
    AbstractBlok::Private::from(
        this
    )->wanted_indices_converters.at(
        input_
    ) = (
        value_
    );
}

void
AbstractFilter::construct
(
    AbstractFilter* this_
)
{
    this_->d_ptr = new Private(this_);
}

AbstractFilter::Private::Private
(
    AbstractFilter* q_ptr_
):
    q_ptr(q_ptr_)
{
}
