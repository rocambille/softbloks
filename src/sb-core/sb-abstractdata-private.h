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
        AbstractData* q_ptr_
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
        DataSet* q_ptr_
    );

    void
    set_index_range
    (
        const IndexRange& value_
    );

    void
    set_defined_indices
    (
        const IndexCollection& value_
    );

    void
    set_wanted_indices
    (
        const IndexCollection& value_
    );

    static
    DataSet::Private*
    from
    (
        const DataSet* this_
    );

    static
    DataSet::Private*
    from
    (
        const SharedDataSet& this_
    );

public:

    DataSet*
    q_ptr;

    sb::AbstractBlok*
    source_blok;

    size_t
    source_index;

    std::vector<sb::AbstractBlok*>
    followers;

    IndexRange
    index_range;

    IndexCollection
    defined_indices;

    IndexCollection
    wanted_indices;

    IndexCollection
    available_indices;

    std::map<double, SharedData>
    data_map;

};

}

#endif // SB_ABSTRACTDATA_PRIVATE_H
