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

    const StepRangeConverter default_step_range_converter =
        [](const std::vector<StepRange>& _sources)
        {
            StepRange result = {{0, 0}};

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

    const StepListConverter default_step_list_converter =
        [](const std::vector<StepList>& _sources)
        {
            StepList result;

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

AbstractBlok::AbstractBlok
(
)
{
    d_ptr = new Private(this);
}

AbstractBlok::Private::Private
(
    AbstractBlok* _q
):
    q_ptr               (_q),
    minimum_input_count (0),
    maximum_input_count (-1),
    minimum_output_count(0),
    maximum_output_count(-1)
{
}

void
AbstractBlok::Private::set_input_count
(
    size_t _value
)
{
    this->minimum_input_count =
    this->maximum_input_count =
        _value;

    this->inputs.resize(_value);
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

    this->inputs.resize(_minimum);
}

void
AbstractBlok::Private::set_output_count
(
    size_t _value
)
{
    this->minimum_output_count =
    this->maximum_output_count =
        _value;

    this->outputs.resize(_value);
}

void
AbstractBlok::Private::set_output_count
(
    size_t _minimum,
    size_t _maximum
)
{
    this->minimum_output_count = _minimum;
    this->maximum_output_count = _maximum;

    this->outputs.resize(_minimum);
}

void
AbstractBlok::Private::update_outputs_step_range
(
)
{
    std::vector<StepRange> step_ranges;

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            step_ranges.push_back(
                input->get_step_range()
            );
        }
    }

    for(SharedDataSet output : this->outputs)
    {
        if(output)
        {
            auto converter = this->step_range_converters.at(
                DataSet::Private::from(
                    output.get()
                )->source_index
            );

            DataSet::Private::from(
                output.get()
            )->set_step_range(
                converter(
                    step_ranges
                )
            );
        }
    }
}

void
AbstractBlok::Private::update_outputs_defined_steps
(
)
{
    std::vector<StepList> step_lists;

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            step_lists.push_back(
                input->get_defined_steps()
            );
        }
    }

    for(SharedDataSet output : this->outputs)
    {
        if(output)
        {
            auto converter = this->defined_steps_converters.at(
                DataSet::Private::from(
                    output.get()
                )->source_index
            );

            DataSet::Private::from(
                output.get()
            )->set_defined_steps(
                converter(
                    step_lists
                )
            );
        }
    }
}

void
AbstractBlok::Private::update_inputs_wanted_steps
(
)
{
    std::vector<StepList> step_lists;

    for(SharedDataSet output : this->outputs)
    {
        if(output)
        {
            step_lists.push_back(
                DataSet::Private::from(
                    output.get()
                )->wanted_steps
            );
        }
    }

    for(SharedDataSet input : this->inputs)
    {
        if(input)
        {
            auto converter = this->wanted_steps_converters.at(
                DataSet::Private::from(
                    input.get()
                )->source_index
            );

            DataSet::Private::from(
                input.get()
            )->set_wanted_steps(
                converter(
                    step_lists
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
