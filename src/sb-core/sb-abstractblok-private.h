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
#ifndef SB_ABSTRACTBLOK_PRIVATE_H
#define SB_ABSTRACTBLOK_PRIVATE_H

#include "sb-abstractblok.h"

#include "sb-abstractdata.h"

namespace sb
{

class AbstractBlok::Private
{

public:

    AbstractBlok::Private
    (
        AbstractBlok* _q
    );

    void
    set_input_count
    (
        size_t _value
    );

    void
    set_input_count
    (
        size_t _minimum,
        size_t _maximum
    );

    void
    set_output_count
    (
        size_t _value
    );

    void
    set_output_count
    (
        size_t _minimum,
        size_t _maximum
    );

    void
    update_outputs_step_range
    (
    );

    void
    update_outputs_defined_steps
    (
    );

    void
    update_inputs_wanted_steps
    (
    );

    static
    AbstractBlok::Private*
    from
    (
        AbstractBlok* _q
    );

public:

    AbstractBlok*
    q_ptr;

    size_t
    minimum_input_count;

    size_t
    maximum_input_count;

    std::vector<SharedDataSet>
    inputs;

    std::vector<StepListConverter>
    wanted_steps_converters;

    size_t
    minimum_output_count;

    size_t
    maximum_output_count;

    std::vector<SharedDataSet>
    outputs;

    std::vector<StepRangeConverter>
    step_range_converters;

    std::vector<StepListConverter>
    defined_steps_converters;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
