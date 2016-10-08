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
#include <sb-core/sb-abstractexecutive.h>

#include <sb-core/sb-abstractexecutive-private.h>

using namespace sb;

AbstractExecutive::AbstractExecutive
(
)
{
    this->d_ptr = new Private(this);
}

AbstractExecutive::~AbstractExecutive
(
)
{
    delete d_ptr;
}

void
AbstractExecutive::execute
(
)
{
    if(!d_ptr->is_executing)
    {
        d_ptr->is_executing = true;

        d_ptr->blok->process();

        d_ptr->is_executing = false;
    }
}

AbstractExecutive::Private::Private
(
    AbstractExecutive* q_ptr_
):
    q_ptr           (q_ptr_),
    blok            (nullptr),
    is_executing    (false)
{
}
