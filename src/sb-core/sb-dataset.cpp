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
#include <sb-core/sb-dataset.h>

#include <sb-core/sb-dataset-private.h>

#include <sb-core/sb-abstractblok-private.h>

using namespace sb;

DataSet::DataSet
(
)
{
    this->d_ptr = new Private(this);
}

DataSet::~DataSet
(
)
{
    delete d_ptr;
}

DataKeyRange
DataSet::get_data_key_range
(
)
const
{
    return d_ptr->data_key_range;
}

DataKeyCollection
DataSet::get_defined_data_keys
(
)
const
{
    return d_ptr->defined_data_keys;
}

DataKeyCollection
DataSet::get_wanted_data_keys
(
)
const
{
    return d_ptr->wanted_data_keys;
}

SharedData
DataSet::get_data
(
    DataKey data_key_
)
const
{
    return d_ptr->data_map.at(data_key_);
}

void
DataSet::set_data
(
    DataKey data_key_,
    const SharedData& value_
)
{
    d_ptr->data_map.at(data_key_) = value_;
}

DataSet::Private::Private
(
    DataSet* q_ptr_
):
    q_ptr       (q_ptr_),
    source_blok (nullptr)
{
    this->set_data_key_range({{0, 0}});
}

void
DataSet::Private::set_data_key_range
(
    const DataKeyRange& value_
)
{
    this->data_key_range = value_;

    for(auto follower : this->followers)
    {
        // TODO : avoid multiple calls on a same blok

        AbstractBlok::Private::from(
            Unmapper::blok(follower)
        )->update_outputs_data_key_range();
    }

    this->defined_data_keys.clear();

    this->defined_data_keys.push_back(
        this->data_key_range[0]
    );

    if(this->data_key_range[0] != this->data_key_range[1])
    {
        this->defined_data_keys.push_back(
            this->data_key_range[1]
        );
    }

    this->set_defined_data_keys(
        this->defined_data_keys
    );
}

void
DataSet::Private::set_defined_data_keys
(
    const DataKeyCollection& value_
)
{
    this->defined_data_keys = value_;

    if(this->defined_data_keys.size() > 0)
    {
        DataKey front_data_key =
            this->defined_data_keys.front();

        DataKey back_data_key =
            this->defined_data_keys.back();

        bool data_key_range_changed = false;

        if(front_data_key < this->data_key_range[0])
        {
            this->data_key_range[0] = front_data_key;

            data_key_range_changed = true;
        }

        if(back_data_key > this->data_key_range[1])
        {
            this->data_key_range[1] = back_data_key;

            data_key_range_changed = true;
        }

        if(data_key_range_changed)
        {
            for(auto follower : this->followers)
            {
                // TODO : avoid multiple calls on a same blok

                AbstractBlok::Private::from(
                    Unmapper::blok(follower)
                )->update_outputs_data_key_range();
            }
        }
    }

    this->wanted_data_keys.clear();
    this->available_data_keys.clear();
    this->data_map.clear();

    for(DataKey data_key : this->defined_data_keys)
    {
        this->data_map[data_key] = nullptr;
    }

    for(auto follower : this->followers)
    {
        // TODO : avoid multiple calls on a same blok

        AbstractBlok::Private::from(
            Unmapper::blok(follower)
        )->update_outputs_defined_data_keys();
    }
}

void
DataSet::Private::set_wanted_data_keys
(
    const DataKeyCollection& value_
)
{
    this->wanted_data_keys = value_;

    // TODO

    AbstractBlok::Private::from(
        this->source_blok
    )->update_inputs_wanted_data_keys();
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
