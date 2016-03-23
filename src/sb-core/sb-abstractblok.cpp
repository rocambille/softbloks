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
#include <sb-core/sb-abstractblok.h>

#include <sb-core/sb-abstractblok-private.h>

#include <algorithm>

#include <sb-core/sb-abstractexecutive-private.h>
#include <sb-core/sb-dataset-private.h>
#include <sb-core/sb-executive.h>

using namespace sb;

namespace Global
{

const DataKeyRangeMapper
default_data_key_range_mapper =
    []
    (
        const std::vector<DataKeyRange>& sources_
    )
    {
        DataKeyRange result = {{0, 0}};

        if(sources_.size() > 0)
        {
            result = sources_[0];

            for(Index i(1); i < sources_.size(); ++i)
            {
                result[0] = std::min(result[0], sources_[i][0]);
                result[1] = std::max(result[1], sources_[i][1]);
            }
        }

        return result;
    };

const DataKeyCollectionMapper
default_data_key_collection_mapper =
    []
    (
        const std::vector<DataKeyCollection>& sources_
    )
    {
        DataKeyCollection result;

        if(sources_.size() > 0)
        {
            result = sources_[0];

            for(Index i(1); i < sources_.size(); ++i)
            {
                result.insert(
                    result.begin(),
                    sources_[i].begin(),
                    sources_[i].end()
                );

                std::unique(
                    result.begin(),
                    result.end()
                );
            }
        }

        return result;
    };

}

AbstractBlok::AbstractBlok
(
)
{
    sb::register_object<PushPullExecutive>();

    this->d_ptr = new Private(this);

    this->use_executive(
        get_type_name<PushPullExecutive>()
    );
}

AbstractBlok::~AbstractBlok
(
)
{
    for(Index i = 0; i < d_ptr->inputs.size(); ++i)
    {
        d_ptr->unlink_input(i);
    }

    for(auto output : d_ptr->outputs)
    {
        DataSet::Private::from(
            output
        )->source_blok = nullptr;
    }

    // d_ptr->executive points to this blok:
    // ensure it is destroyed first

    d_ptr->executive.reset();

    delete d_ptr;
}

Size
AbstractBlok::get_minimum_input_count
(
)
const
{
    return d_ptr->minimum_input_count;
}

Size
AbstractBlok::get_maximum_input_count
(
)
const
{
    return d_ptr->maximum_input_count;
}

Size
AbstractBlok::get_input_count
(
)
const
{
    return d_ptr->inputs.size();
}

Size
AbstractBlok::get_minimum_output_count
(
)
const
{
    return d_ptr->minimum_output_count;
}

Size
AbstractBlok::get_maximum_output_count
(
)
const
{
    return d_ptr->maximum_output_count;
}

Size
AbstractBlok::get_output_count
(
)
const
{
    return d_ptr->outputs.size();
}

void
AbstractBlok::use_executive
(
    const std::string& name_
)
{
    d_ptr->executive = sb::create_unique_executive(name_);

    if(!d_ptr->executive)
    {
        throw std::invalid_argument(
            "fatal: AbstractBlok::use_executive(invalid executive name)"
        );
    }

    AbstractExecutive::Private::from(
        d_ptr->executive
    )->blok = this;
}

void
AbstractBlok::pull_input
(
    Index index_
)
{
    // AbstractBlok::Private::lock_input calls this method:
    // don't call it here or it will cause infinite recursion

    auto input_d_ptr = DataSet::Private::from(
        d_ptr->inputs.at(index_).lock()
    );

    AbstractBlok::Private::from(
        input_d_ptr->source_blok
    )->executive->on_output_pulled(
        input_d_ptr->source_index
    );
}

void
AbstractBlok::push_output
(
    Index index_
)
{
    auto output_d_ptr = DataSet::Private::from(
        d_ptr->outputs.at(index_)
    );

    for(auto follower : output_d_ptr->followers)
    {
        AbstractBlok::Private::from(
            Unmapper::blok(follower)
        )->executive->on_input_pushed(
            Unmapper::input_index(follower)
        );
    }
}

AbstractBlok::Private::Private
(
    AbstractBlok* q_ptr_
):
    q_ptr               (q_ptr_),
    minimum_input_count (0),
    maximum_input_count (sb::MAX_SIZE),
    minimum_output_count(0),
    maximum_output_count(sb::MAX_SIZE)
{
    sb::register_object<DataSet>();
}

void
AbstractBlok::Private::set_input_count
(
    Size value_
)
{
    this->set_input_count(
        value_,
        value_
    );
}

void
AbstractBlok::Private::set_input_count
(
    Size minimum_,
    Size maximum_
)
{
    this->minimum_input_count = minimum_;
    this->maximum_input_count = maximum_;

    this->inputs.resize(minimum_);

    this->inputs_format.resize(
        this->inputs.size(),
        UNDEFINED_OBJECT_FORMAT
    );

    this->wanted_data_keys_mappers.resize(
        this->inputs.size(),
        Global::default_data_key_collection_mapper
    );
}

void
AbstractBlok::Private::set_input_format
(
    Index index_,
    const ObjectFormat& format_
)
{
    this->inputs_format[index_] = format_;
}

void
AbstractBlok::Private::set_output_count
(
    Size value_
)
{
    this->set_output_count(
        value_,
        value_
    );
}

void
AbstractBlok::Private::set_output_count
(
    Size minimum_,
    Size maximum_
)
{
    Size previous_output_count =
        this->outputs.size();

    this->minimum_output_count = minimum_;
    this->maximum_output_count = maximum_;

    this->outputs.resize(minimum_);

    for(Index i(previous_output_count); i < this->outputs.size(); ++i)
    {
        SharedDataSet data_set = sb::create_shared_data_set(
            get_type_name<DataSet>()
        );

        auto data_set_d_ptr = DataSet::Private::from(
            data_set
        );

        data_set_d_ptr->source_blok = q_ptr;
        data_set_d_ptr->source_index = i;

        this->outputs[i] = data_set;
    }

    this->data_key_range_mappers.resize(
        this->outputs.size(),
        Global::default_data_key_range_mapper
    );

    this->defined_data_keys_mappers.resize(
        this->outputs.size(),
        Global::default_data_key_collection_mapper
    );
}

void
AbstractBlok::Private::set_output_format
(
    Index index_,
    const ObjectFormat& format_
)
{
}

void
AbstractBlok::Private::update_outputs_data_key_range
(
)
{
    std::vector<DataKeyRange> data_key_ranges;

    for(auto input : this->inputs)
    {
        auto locked_input = input.lock();

        if(locked_input)
        {
            data_key_ranges.push_back(
                locked_input->get_data_key_range()
            );
        }
        else
        {
            data_key_ranges.push_back(
                DataKeyRange({{0, 0}})
            );
        }
    }

    for(auto output : this->outputs)
    {
        auto mapper = this->data_key_range_mappers.at(
            DataSet::Private::from(
                output
            )->source_index
        );

        DataSet::Private::from(
            output
        )->set_data_key_range(
            mapper(
                data_key_ranges
            )
        );
    }
}

void
AbstractBlok::Private::update_outputs_defined_data_keys
(
)
{
    std::vector<DataKeyCollection> data_key_collections;

    for(auto input : this->inputs)
    {
        auto locked_input = input.lock();

        if(locked_input)
        {
            data_key_collections.push_back(
                locked_input->get_defined_data_keys()
            );
        }
        else
        {
            data_key_collections.push_back(
                DataKeyCollection()
            );
        }
    }

    for(auto output : this->outputs)
    {
        auto mapper = this->defined_data_keys_mappers.at(
            DataSet::Private::from(
                output
            )->source_index
        );

        DataSet::Private::from(
            output
        )->set_defined_data_keys(
            mapper(
                data_key_collections
            )
        );
    }
}

void
AbstractBlok::Private::update_inputs_wanted_data_keys
(
)
{
    std::vector<DataKeyCollection> data_key_collections;

    for(auto output : this->outputs)
    {
        data_key_collections.push_back(
            DataSet::Private::from(
                output
            )->wanted_data_keys
        );
    }

    for(auto input : this->inputs)
    {
        auto locked_input = input.lock();

        if(locked_input)
        {
            auto mapper = this->wanted_data_keys_mappers.at(
                DataSet::Private::from(
                    locked_input
                )->source_index
            );

            DataSet::Private::from(
                locked_input
            )->set_wanted_data_keys(
                mapper(
                    data_key_collections
                )
            );
        }
    }
}

SharedDataSet
AbstractBlok::Private::lock_input
(
    Index index_
)
const
{
    q_ptr->pull_input(index_);

    return this->inputs.at(index_).lock();
}

bool
AbstractBlok::Private::set_input
(
    Index index_,
    const SharedDataSet& value_
)
{
    bool ok = false;

    if(
        index_ >= this->inputs.size() &&
        index_ < this->maximum_input_count
    )
    {
        this->inputs.resize(index_+1);

        this->inputs_format.resize(
            this->inputs.size(),
            UNDEFINED_OBJECT_FORMAT
        );

        this->wanted_data_keys_mappers.resize(
            this->inputs.size(),
            Global::default_data_key_collection_mapper
        );
    }

    ObjectFormat input_format = UNDEFINED_OBJECT_FORMAT;

    if(index_ < this->inputs_format.size())
    {
        input_format = this->inputs_format.at(index_);
    }

    if(
        value_ == nullptr ||
        value_->get_instance_format() >> input_format
    )
    {
        ok = true;

        this->unlink_input(index_);

        this->inputs[index_] = value_;

        if(value_)
        {
            // register this blok as a follower

            DataSet::Private::from(
                value_
            )->followers.emplace(
                q_ptr, index_
            );
        }

        // update data keys

        this->update_outputs_data_key_range();
        this->update_outputs_defined_data_keys();

        this->update_inputs_wanted_data_keys();
    }

    return ok;
}

void
AbstractBlok::Private::unlink_input
(
    Index index_
)
{
    auto input = this->inputs[index_].lock();

    if(input)
    {
        // unregister this blok from previous input's followers

        auto input_d_ptr = DataSet::Private::from(
            input
        );

        auto erase_candidates = 
            input_d_ptr->followers.equal_range(
                q_ptr
            );

        auto candidate = erase_candidates.first;

        while(candidate != erase_candidates.second)
        {
            if(Unmapper::input_index(*candidate) == index_)
            {
                candidate = input_d_ptr->followers.erase(candidate);
            }
            else
            {
                ++candidate;
            }
        }
    }
}

AbstractBlok::Private*
AbstractBlok::Private::from
(
    const AbstractBlok* this_
)
{
    return this_->d_ptr;
}

AbstractBlok::Private*
AbstractBlok::Private::from
(
    const UniqueBlok& this_
)
{
    return this_->d_ptr;
}
