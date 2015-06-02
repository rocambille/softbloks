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
#include <sb-core/sb-dataset.h>

#include <sb-core/sb-dataset-private.h>

#include <sb-core/sb-abstractblok-private.h>

using namespace sb;

DataSet::~DataSet
(
)
{
    delete d_ptr;
}

IndexRange
DataSet::get_index_range
(
)
const
{
    return d_ptr->index_range;
}

IndexCollection
DataSet::get_defined_indices
(
)
const
{
    return d_ptr->defined_indices;
}

IndexCollection
DataSet::get_wanted_indices
(
)
const
{
    return d_ptr->wanted_indices;
}

SharedData
DataSet::get_data
(
    double index_
)
const
{
    return d_ptr->data_map.at(index_);
}

void
DataSet::set_data
(
    double index_,
    const SharedData& value_
)
{
    d_ptr->data_map.at(index_) = value_;
}

void
DataSet::construct
(
    DataSet* this_
)
{
    this_->d_ptr = new Private(this_);
}

DataSet::Private::Private
(
    DataSet* q_ptr_
):
    q_ptr       (q_ptr_),
    source_blok (nullptr)
{
    this->set_index_range({{0, 0}});
}

void
DataSet::Private::set_index_range
(
    const IndexRange& value_
)
{
    this->index_range = value_;

    for(auto follower : this->followers)
    {
        AbstractBlok::Private::from(
            follower
        )->update_outputs_index_range();
    }

    this->defined_indices.clear();

    this->defined_indices.push_back(
        this->index_range[0]
    );

    if(this->index_range[0] != this->index_range[1])
    {
        this->defined_indices.push_back(
            this->index_range[1]
        );
    }

    this->set_defined_indices(
        this->defined_indices
    );
}

void
DataSet::Private::set_defined_indices
(
    const IndexCollection& value_
)
{
    this->defined_indices = value_;

    if(this->defined_indices.size() > 0)
    {
        double front_index =
            this->defined_indices.front();

        double back_index =
            this->defined_indices.back();

        bool index_range_changed = false;

        if(front_index < this->index_range[0])
        {
            this->index_range[0] = front_index;

            index_range_changed = true;
        }

        if(back_index > this->index_range[1])
        {
            this->index_range[1] = back_index;

            index_range_changed = true;
        }

        if(index_range_changed)
        {
            for(auto follower : this->followers)
            {
                AbstractBlok::Private::from(
                    follower
                )->update_outputs_index_range();
            }
        }
    }

    this->wanted_indices.clear();
    this->available_indices.clear();
    this->data_map.clear();

    for(double index : this->defined_indices)
    {
        this->data_map[index] = nullptr;
    }

    for(auto follower : this->followers)
    {
        AbstractBlok::Private::from(
            follower
        )->update_outputs_defined_indices();
    }
}

void
DataSet::Private::set_wanted_indices
(
    const IndexCollection& value_
)
{
    this->wanted_indices = value_;

    // TODO

    AbstractBlok::Private::from(
        this->source_blok
    )->update_inputs_wanted_indices();
}

DataSet::Private*
DataSet::Private::from
(
    const DataSet* this_
)
{
    return this_->d_ptr;
}

DataSet::Private*
DataSet::Private::from
(
    const SharedDataSet& this_
)
{
    return this_->d_ptr;
}
