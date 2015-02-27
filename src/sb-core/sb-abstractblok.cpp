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
#include "sb-abstractblok.h"

#include "sb-abstractblok-private.h"

#include <algorithm>

#include "sb-abstractdata-private.h"

using namespace sb;

namespace Global
{

const IndexRangeConverter
default_index_range_converter =
    []
    (
        const std::vector<IndexRange>& sources_
    )
    {
        IndexRange result = {{0, 0}};

        if(sources_.size() > 0)
        {
            result = sources_[0];

            for(size_t i(1); i < sources_.size(); ++i)
            {
                result[0] = std::min(result[0], sources_[i][0]);
                result[1] = std::max(result[1], sources_[i][1]);
            }
        }

        return result;
    };

const IndexCollectionConverter
default_index_collection_converter =
    []
    (
        const std::vector<IndexCollection>& sources_
    )
    {
        IndexCollection result;

        if(sources_.size() > 0)
        {
            result = sources_[0];

            for(size_t i(1); i < sources_.size(); ++i)
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

AbstractBlok::~AbstractBlok
(
)
{
    for(SharedDataSet output : d_ptr->outputs)
    {
        DataSet::Private::from(
            output
        )->source_blok = nullptr;
    }

    delete d_ptr;
}

size_t
AbstractBlok::get_minimum_input_count
(
)
const
{
    return d_ptr->minimum_input_count;
}

size_t
AbstractBlok::get_maximum_input_count
(
)
const
{
    return d_ptr->maximum_input_count;
}

size_t
AbstractBlok::get_input_count
(
)
const
{
    return d_ptr->inputs.size();
}

size_t
AbstractBlok::get_minimum_output_count
(
)
const
{
    return d_ptr->minimum_output_count;
}

size_t
AbstractBlok::get_maximum_output_count
(
)
const
{
    return d_ptr->maximum_output_count;
}

size_t
AbstractBlok::get_output_count
(
)
const
{
    return d_ptr->outputs.size();
}

void
AbstractBlok::construct
(
    AbstractBlok* this_
)
{
    this_->d_ptr = new Private(this_);
}

AbstractBlok::Private::Private
(
    AbstractBlok* q_ptr_
):
    q_ptr               (q_ptr_),
    minimum_input_count (0),
    maximum_input_count (sb::infinity),
    minimum_output_count(0),
    maximum_output_count(sb::infinity)
{
    sb::register_object<DataSet>();
}

void
AbstractBlok::Private::set_input_count
(
    size_t value_
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
    size_t minimum_,
    size_t maximum_
)
{
    this->minimum_input_count = minimum_;
    this->maximum_input_count = maximum_;

    this->inputs.resize(minimum_, nullptr);

    this->inputs_format.resize(
        this->inputs.size(),
        undefined_object
    );

    this->wanted_indices_converters.resize(
        this->inputs.size(),
        Global::default_index_collection_converter
    );
}

void
AbstractBlok::Private::set_input_format
(
    size_t index_,
    const ObjectInformation& format_
)
{
    this->inputs_format[index_] = format_;
}

void
AbstractBlok::Private::set_output_count
(
    size_t value_
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
    size_t minimum_,
    size_t maximum_
)
{
    size_t previous_output_count =
        this->outputs.size();

    this->minimum_output_count = minimum_;
    this->maximum_output_count = maximum_;

    this->outputs.resize(minimum_);

    for(size_t i(previous_output_count); i < this->outputs.size(); ++i)
    {
        SharedDataSet data_set = sb::create<DataSet>(
            DataSet::get_object_name()
        );

        auto data_set_d_ptr = DataSet::Private::from(
            data_set
        );

        data_set_d_ptr->source_blok = q_ptr;
        data_set_d_ptr->source_index = i;

        this->outputs[i] = data_set;
    }

    this->index_range_converters.resize(
        this->outputs.size(),
        Global::default_index_range_converter
    );

    this->defined_indices_converters.resize(
        this->outputs.size(),
        Global::default_index_collection_converter
    );
}

void
AbstractBlok::Private::set_output_format
(
    size_t index_,
    const ObjectInformation& format_
)
{
}

void
AbstractBlok::Private::update_outputs_index_range
(
)
{
    std::vector<IndexRange> index_ranges;

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            index_ranges.push_back(
                input->get_index_range()
            );
        }
        else
        {
            index_ranges.push_back(
                IndexRange({{0, 0}})
            );
        }
    }

    for(SharedDataSet output : this->outputs)
    {
        auto converter = this->index_range_converters.at(
            DataSet::Private::from(
                output
            )->source_index
        );

        DataSet::Private::from(
            output
        )->set_index_range(
            converter(
                index_ranges
            )
        );
    }
}

void
AbstractBlok::Private::update_outputs_defined_indices
(
)
{
    std::vector<IndexCollection> index_collections;

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            index_collections.push_back(
                input->get_defined_indices()
            );
        }
        else
        {
            index_collections.push_back(
                IndexCollection()
            );
        }
    }

    for(SharedDataSet output : this->outputs)
    {
        auto converter = this->defined_indices_converters.at(
            DataSet::Private::from(
                output
            )->source_index
        );

        DataSet::Private::from(
            output
        )->set_defined_indices(
            converter(
                index_collections
            )
        );
    }
}

void
AbstractBlok::Private::update_inputs_wanted_indices
(
)
{
    std::vector<IndexCollection> index_collections;

    for(SharedDataSet output : this->outputs)
    {
        index_collections.push_back(
            DataSet::Private::from(
                output
            )->wanted_indices
        );
    }

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            auto converter = this->wanted_indices_converters.at(
                DataSet::Private::from(
                    input
                )->source_index
            );

            DataSet::Private::from(
                input
            )->set_wanted_indices(
                converter(
                    index_collections
                )
            );
        }
    }
}

SharedDataSet
AbstractBlok::Private::get_input
(
    size_t index_
)
const
{
    return this->inputs.at(index_);
}

bool
AbstractBlok::Private::set_input
(
    size_t index_,
    const SharedDataSet& value_
)
{
    bool ok = false;

    if(
        index_ >= this->inputs.size() &&
        index_ < this->maximum_input_count
    )
    {
        this->inputs.resize(index_+1, nullptr);

        this->inputs_format.resize(
            this->inputs.size(),
            undefined_object
        );

        this->wanted_indices_converters.resize(
            this->inputs.size(),
            Global::default_index_collection_converter
        );
    }

    ObjectInformation input_format = undefined_object;

    if(index_ < this->inputs_format.size())
    {
        input_format = this->inputs_format.at(index_);
    }

    if(
        value_ == nullptr ||
        value_->get_instance_information() >> input_format
    )
    {
        ok = true;

        this->inputs[index_] = value_;

        this->update_outputs_index_range();
        this->update_outputs_defined_indices();

        this->update_inputs_wanted_indices();
    }

    return ok;
}

SharedDataSet
AbstractBlok::Private::get_output
(
    size_t index_
)
const
{
    return this->outputs.at(index_);
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
    const SharedBlok& this_
)
{
    return this_->d_ptr;
}
