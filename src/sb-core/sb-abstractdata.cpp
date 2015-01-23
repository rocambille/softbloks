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

#include "sb-abstractblok-private.h"

using namespace sb;

AbstractData::~AbstractData
(
)
{
    delete d_ptr;
}

AbstractData::AbstractData
(
)
{
    d_ptr = new Private(this);
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
    return d_ptr->data_map.at(_key);
}

void
DataSet::set_step
(
    double _key,
    const SharedData& _value
)
{
    d_ptr->data_map.at(_key) = _value;
}

DataSet::Private::Private
(
    DataSet* _q
):
    q_ptr       (_q),
    source_blok (nullptr)
{
    this->set_step_range({{0, 0}});
}

void
DataSet::Private::set_step_range
(
    const StepRange& _value
)
{
    this->step_range = _value;

    for(auto follower : this->followers)
    {
        AbstractBlok::Private::from(
            follower
        )->update_outputs_step_range();
    }

    this->defined_steps.clear();

    this->defined_steps.push_back(
        this->step_range[0]
    );

    if(this->step_range[0] != this->step_range[1])
    {
        this->defined_steps.push_back(
            this->step_range[1]
        );
    }

    this->set_defined_steps(this->defined_steps);
}

void
DataSet::Private::set_defined_steps
(
    const StepList& _value
)
{
    this->defined_steps = _value;

    if(this->defined_steps.size() > 0)
    {
        double front_step = this->defined_steps.front();
        double back_step = this->defined_steps.back();

        bool step_range_changed = false;

        if(front_step < this->step_range[0])
        {
            this->step_range[0] = front_step;

            step_range_changed = true;
        }

        if(back_step < this->step_range[1])
        {
            this->step_range[1] = back_step;

            step_range_changed = true;
        }

        if(step_range_changed)
        {
            for(auto follower : this->followers)
            {
                AbstractBlok::Private::from(
                    follower
                )->update_outputs_step_range();
            }
        }
    }

    this->wanted_steps.clear();
    this->available_steps.clear();
    this->data_map.clear();

    for(double step : this->defined_steps)
    {
        this->data_map[step] = nullptr;
    }

    for(auto follower : this->followers)
    {
        AbstractBlok::Private::from(
            follower
        )->update_outputs_defined_steps();
    }
}

void
DataSet::Private::set_wanted_steps
(
    const StepList& _value
)
{
    this->wanted_steps = _value;

    // TODO

    AbstractBlok::Private::from(
        this->source_blok
    )->update_inputs_wanted_steps();
}

DataSet::Private*
DataSet::Private::from
(
    const DataSet* _q
)
{
    return _q->d_ptr;
}
