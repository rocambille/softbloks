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

PushExecutive::~PushExecutive
(
)
{
    delete d_ptr;
}

void
PushExecutive::on_input_pushed
(
    size_t /*index_*/
)
{
    this->execute();
}

void
PushExecutive::on_output_pulled
(
    size_t /*index_*/
)
{
}

void
PushExecutive::construct
(
    PushExecutive* this_
)
{
    this_->d_ptr = new Private(this_);
}

PushExecutive::Private::Private
(
    PushExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}

//////////////////////////////////////////////////////////////////////////////

PullExecutive::~PullExecutive
(
)
{
    delete d_ptr;
}

void
PullExecutive::on_input_pushed
(
    size_t /*index_*/
)
{
}

void
PullExecutive::on_output_pulled
(
    size_t /*index_*/
)
{
    this->execute();
}

void
PullExecutive::construct
(
    PullExecutive* this_
)
{
    this_->d_ptr = new Private(this_);
}

PullExecutive::Private::Private
(
    PullExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}

//////////////////////////////////////////////////////////////////////////////

PushPullExecutive::~PushPullExecutive
(
)
{
    delete d_ptr;
}

void
PushPullExecutive::on_input_pushed
(
    size_t /*index_*/
)
{
    this->execute();
}

void
PushPullExecutive::on_output_pulled
(
    size_t /*index_*/
)
{
    this->execute();
}

void
PushPullExecutive::construct
(
    PushPullExecutive* this_
)
{
    this_->d_ptr = new Private(this_);
}

PushPullExecutive::Private::Private
(
    PushPullExecutive* q_ptr_
):
    q_ptr(q_ptr_)
{
}
