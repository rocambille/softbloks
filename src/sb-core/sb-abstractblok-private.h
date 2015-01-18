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
    set_step_range
    (
        size_t _output,
        const StepRange& _value
    );

    void
    set_defined_steps
    (
        size_t _output,
        const StepList& _value
    );

    void
    set_wanted_steps
    (
        size_t _input,
        const StepList& _value
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

    std::vector<SharedData>
    inputs;

    size_t
    minimum_output_count;

    size_t
    maximum_output_count;

    std::vector<SharedData>
    outputs;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
