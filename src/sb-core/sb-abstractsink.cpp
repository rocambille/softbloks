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
    size_t index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->inputs.at(index_);
}

bool
AbstractSink::set_input
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

void
AbstractSink::set_wanted_indices
(
    size_t input_,
    const IndexCollection& value_
)
{
    DataSet::Private::from(
        this->get_input(
            input_
        )
    )->set_wanted_indices(
        value_
    );
}

void
AbstractSink::set_input_count
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
AbstractSink::set_input_count
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
AbstractSink::set_input_format
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
AbstractSink::construct
(
    AbstractSink* this_
)
{
    this_->d_ptr = new Private(this_);
}

AbstractSink::Private::Private
(
    AbstractSink* q_ptr_
):
    q_ptr(q_ptr_)
{
}
