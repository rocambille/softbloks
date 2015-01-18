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
#include "sb-abstractdata.h"

#include "sb-abstractdata-private.h"

using namespace sb;

AbstractData::AbstractData
(
)
{
    d_ptr = new Private(this);
}

AbstractData::~AbstractData
(
)
{
    delete d_ptr;
}

AbstractData::Private::Private
(
    AbstractData* _q
):
    q_ptr(_q)
{
}

DataSet::DataSet
(
)
{
    d_ptr = new Private(this);
}

DataSet::~DataSet
(
)
{
    delete d_ptr;
}

StepRange
DataSet::get_step_range
(
)
const
{
    return d_ptr->step_range;
}

StepList
DataSet::get_defined_steps
(
)
const
{
    return d_ptr->defined_steps;
}

SharedData
DataSet::get_step
(
    double _key
)
const
{
    return SharedData();
}

void
DataSet::set_step
(
    double _key,
    const SharedData& _value
)
{

}

DataSet::Private::Private
(
    DataSet* _q
):
    q_ptr       (_q),
    source_blok (nullptr),
    step_range  ({{0, 0}})
{
}
