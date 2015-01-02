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

using namespace sb;

AbstractSource::Private::Private
(
    AbstractSource* _q
):
    q_ptr(_q)
{
}

AbstractSource::AbstractSource
(
)
{
    d_ptr = new Private(this);
}

AbstractSource::~AbstractSource
(
)
{
    delete d_ptr;
}
