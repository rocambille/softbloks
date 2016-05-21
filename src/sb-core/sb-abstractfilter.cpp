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

AbstractFilter::AbstractFilter
(
)
{
    this->d_ptr = new Private(this);
}

AbstractFilter::~AbstractFilter
(
)
{
    delete d_ptr;
}

SharedData
AbstractFilter::lock_input
(
    Index index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->lock_input(index_);
}

bool
AbstractFilter::set_input
(
    Index index_,
    const SharedData& value_
)
{
    return AbstractBlok::Private::from(
        this
    )->set_input(
        index_,
        value_
    );
}

SharedData
AbstractFilter::get_output
(
    Index index_
)
const
{
    return AbstractBlok::Private::from(
        this
    )->outputs.at(index_);
}

AbstractFilter::Private::Private
(
    AbstractFilter* q_ptr_
):
    q_ptr(q_ptr_)
{
}
