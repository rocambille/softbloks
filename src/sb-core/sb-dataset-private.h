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
#ifndef SB_DATASET_PRIVATE_H
#define SB_DATASET_PRIVATE_H

#include <sb-core/sb-dataset.h>

#include <sb-core/sb-abstractblok.h>

#include <unordered_map>

namespace sb
{

using FollowerCollection =
    std::unordered_multimap<AbstractBlok*, size_t>;

namespace Unmapper
{

    inline
    AbstractBlok*
    blok
    (
        const FollowerCollection::value_type& value_
    )
    {
        return value_.first;
    }

    inline
    size_t
    input_index
    (
        const FollowerCollection::value_type& value_
    )
    {
        return value_.second;
    }

}

class SB_DECL_HIDDEN DataSet::Private
{

public:

    Private
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
    Private*
    from
    (
        const DataSet* this_
    );

    static
    Private*
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

    FollowerCollection
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

using WeakDataSet = Weak<DataSet>;

}

#endif // SB_DATASET_PRIVATE_H
