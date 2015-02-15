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

#include "sb-abstractdata-private.h"

using namespace sb;

#define SB_MIN(_a, _b) ((_a) < (_b)) ? (_a) : (_b)
#define SB_MAX(_a, _b) ((_a) > (_b)) ? (_a) : (_b)

namespace Global
{

    const IndexRangeConverter default_index_range_converter = []
        (
            const std::vector<IndexRange>& _sources
        )
        {
            IndexRange result = {{0, 0}};

            if(_sources.size() > 0)
            {
                result = _sources[0];

                for(size_t i(1); i < _sources.size(); ++i)
                {
                    result[0] = SB_MIN(result[0], _sources[i][0]);
                    result[1] = SB_MAX(result[1], _sources[i][1]);
                }
            }

            return result;
        };

    const IndexCollectionConverter default_index_collection_converter = []
        (
            const std::vector<IndexCollection>& _sources
        )
        {
            IndexCollection result;

            if(_sources.size() > 0)
            {
                result = _sources[0];

                for(size_t i(1); i < _sources.size(); ++i)
                {
                    result.insert(
                        result.begin(),
                        _sources[i].begin(),
                        _sources[i].end()
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
            output.get()
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
    AbstractBlok* _this
)
{
    _this->d_ptr = new Private(_this);
}

AbstractBlok::Private::Private
(
    AbstractBlok* _q
):
    q_ptr               (_q),
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
    size_t _value
)
{
    this->set_input_count(
        _value,
        _value
    );
}

void
AbstractBlok::Private::set_input_count
(
    size_t _minimum,
    size_t _maximum
)
{
    this->minimum_input_count = _minimum;
    this->maximum_input_count = _maximum;

    this->inputs.resize(_minimum, nullptr);

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
    size_t _index,
    const ObjectInformation& _format
)
{
    this->inputs_format[_index] = _format;
}

void
AbstractBlok::Private::set_output_count
(
    size_t _value
)
{
    this->set_output_count(
        _value,
        _value
    );
}

void
AbstractBlok::Private::set_output_count
(
    size_t _minimum,
    size_t _maximum
)
{
    size_t previous_output_count =
        this->outputs.size();

    this->minimum_output_count = _minimum;
    this->maximum_output_count = _maximum;

    this->outputs.resize(_minimum);

    for(size_t i(previous_output_count); i < this->outputs.size(); ++i)
    {
        SharedDataSet data_set = sb::create<DataSet>(
            DataSet::get_object_name()
        );

        auto data_set_d_ptr = DataSet::Private::from(
            data_set.get()
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
    size_t _index,
    const ObjectInformation& _format
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
                output.get()
            )->source_index
        );

        DataSet::Private::from(
            output.get()
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
                output.get()
            )->source_index
        );

        DataSet::Private::from(
            output.get()
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
                output.get()
            )->wanted_indices
        );
    }

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            auto converter = this->wanted_indices_converters.at(
                DataSet::Private::from(
                    input.get()
                )->source_index
            );

            DataSet::Private::from(
                input.get()
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
    size_t _index
)
const
{
    return this->inputs.at(_index);
}

bool
AbstractBlok::Private::set_input
(
    size_t _index,
    const SharedDataSet& _value
)
{
    bool ok = false;

    if(
        _index >= this->inputs.size() &&
        _index < this->maximum_input_count
    )
    {
        this->inputs.resize(_index+1, nullptr);

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

    if(_index < this->inputs_format.size())
    {
        input_format = this->inputs_format.at(_index);
    }

    if(
        _value == nullptr ||
        _value->get_instance_information() >> input_format
    )
    {
        ok = true;

        this->inputs[_index] = _value;

        this->update_outputs_index_range();
        this->update_outputs_defined_indices();

        this->update_inputs_wanted_indices();
    }

    return ok;
}

SharedDataSet
AbstractBlok::Private::get_output
(
    size_t _index
)
const
{
    return this->outputs.at(_index);
}

AbstractBlok::Private*
AbstractBlok::Private::from
(
    const AbstractBlok* _q
)
{
    return _q->d_ptr;
}
