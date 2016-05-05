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
#include <sb-core/sb-abstractsink.h>

#include <sb-core/sb-abstractsink-private.h>

#include <sb-core/sb-abstractblok-private.h>
#include <sb-core/sb-dataset-private.h>

using namespace sb;

AbstractSink::AbstractSink
(
)
{
    this->d_ptr = new Private(this);
}

AbstractSink::~AbstractSink
(
)
{
    delete d_ptr;
}

SharedDataSet
AbstractSink::lock_input
(
    Index index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->inputs.at(index_).lock();
}

bool
AbstractSink::set_input
(
    Index index_,
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
AbstractSink::set_wanted_data_keys
(
    Index input_,
    const DataKeyCollection& value_
)
{
    DataSet::Private::from(
        this->lock_input(
            input_
        )
    )->set_wanted_data_keys(
        value_
    );
}

void
AbstractSink::set_input_count
(
    Size value_
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
    Size minimum_,
    Size maximum_
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
    Index index_,
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

AbstractSink::Private::Private
(
    AbstractSink* q_ptr_
):
    q_ptr(q_ptr_)
{
}
