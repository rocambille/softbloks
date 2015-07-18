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
#include <sb-core/sb-abstractsource.h>

#include <sb-core/sb-abstractsource-private.h>

#include <sb-core/sb-abstractblok-private.h>
#include <sb-core/sb-dataset-private.h>

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
    size_t index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->outputs.at(index_);
}

void
AbstractSource::set_output_count
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
AbstractSource::set_output_count
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
AbstractSource::set_output_format
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
AbstractSource::set_index_range
(
    size_t output_,
    const IndexRange& value_
)
{
    DataSet::Private::from(
        this->get_output(
            output_
        )
    )->set_index_range(
        value_
    );
}

void
AbstractSource::set_defined_indices
(
    size_t output_,
    const IndexCollection& value_
)
{
    DataSet::Private::from(
        this->get_output(
            output_
        )
    )->set_defined_indices(
        value_
    );
}

void
AbstractSource::construct
(
    AbstractSource* this_
)
{
    this_->d_ptr = new Private(this_);
}

AbstractSource::Private::Private
(
    AbstractSource* q_ptr_
):
    q_ptr(q_ptr_)
{
}
