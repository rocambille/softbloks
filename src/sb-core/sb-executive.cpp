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
#include <sb-core/sb-executive.h>

#include <sb-core/sb-executive-private.h>

using namespace sb;

PushExecutive::PushExecutive
(
)
{
    this->d_ptr = new Private(this);
}

PushExecutive::~PushExecutive
(
)
{
    delete d_ptr;
}

void
PushExecutive::on_input_pushed
(
    Index /*index_*/
)
{
    this->execute();
}

void
PushExecutive::on_output_pulled
(
    Index /*index_*/
)
{
}

PushExecutive::Private::Private
(
    PushExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}

//////////////////////////////////////////////////////////////////////////////

PullExecutive::PullExecutive
(
)
{
    this->d_ptr = new Private(this);
}

PullExecutive::~PullExecutive
(
)
{
    delete d_ptr;
}

void
PullExecutive::on_input_pushed
(
    Index /*index_*/
)
{
}

void
PullExecutive::on_output_pulled
(
    Index /*index_*/
)
{
    this->execute();
}

PullExecutive::Private::Private
(
    PullExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}

//////////////////////////////////////////////////////////////////////////////

PushPullExecutive::PushPullExecutive
(
)
{
    this->d_ptr = new Private(this);
}

PushPullExecutive::~PushPullExecutive
(
)
{
    delete d_ptr;
}

void
PushPullExecutive::on_input_pushed
(
    Index /*index_*/
)
{
    this->execute();
}

void
PushPullExecutive::on_output_pulled
(
    Index /*index_*/
)
{
    this->execute();
}

PushPullExecutive::Private::Private
(
    PushPullExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}
