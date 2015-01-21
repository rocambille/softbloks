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
#ifndef SB_ABSTRACTDATA_PRIVATE_H
#define SB_ABSTRACTDATA_PRIVATE_H

#include "sb-abstractdata.h"

#include "sb-abstractblok.h"

namespace sb
{

class AbstractData::Private
{

public:

    AbstractData::Private
    (
        AbstractData* _q
    );

public:

    AbstractData*
    q_ptr;

};

class DataSet::Private
{

public:

    DataSet::Private
    (
        DataSet* _q
    );

    void
    set_step_range
    (
        const StepRange& _value
    );

    void
    set_defined_steps
    (
        const StepList& _value
    );

    void
    set_wanted_steps
    (
        const StepList& _value
    );

    static
    DataSet::Private*
    from
    (
        DataSet* _q
    );

public:

    DataSet*
    q_ptr;

    sb::AbstractBlok*
    source_blok;

    size_t
    source_index;

    std::list<sb::AbstractBlok*>
    followers;

    StepRange
    step_range;

    StepList
    defined_steps;

    StepList
    wanted_steps;

    StepList
    available_steps;

    std::map<double, SharedData>
    data_map;

};

}

#endif // SB_ABSTRACTDATA_PRIVATE_H
