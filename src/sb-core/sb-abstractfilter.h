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
#ifndef SB_ABSTRACTFILTER_H
#define SB_ABSTRACTFILTER_H

#include "sb-abstractblok.h"

namespace sb
{

class SB_CORE_API AbstractFilter : public AbstractBlok
{

public:

    class Private;

    virtual
    ~AbstractFilter
    (
    );

    SharedDataSet
    get_input
    (
        size_t _index
    )
    const;

    SharedDataSet
    get_output
    (
        size_t _index
    )
    const;

protected:

    AbstractFilter
    (
    );

    void
    set_input_count
    (
        int _value
    );

    void
    set_input_count
    (
        int _minimum,
        int _maximum
    );

    void
    set_output_count
    (
        int _value
    );

    void
    set_output_count
    (
        int _minimum,
        int _maximum
    );

    void
    set_step_range_converter
    (
        size_t _output,
        const StepRangeConverter& _value
    );

    void
    set_defined_steps_converter
    (
        size_t _output,
        const StepListConverter& _value
    );

    void
    set_wanted_steps_converter
    (
        size_t _input,
        const StepListConverter& _value
    );

private:

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTFILTER_H
