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

    SB_DECLARE_OBJECT(AbstractFilter, "sb::AbstractFilter")

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

    bool
    set_input
    (
        size_t _index,
        const SharedDataSet& _value
    );

    SharedDataSet
    get_output
    (
        size_t _index
    )
    const;

protected:

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
    set_input_format
    (
        size_t _index,
        const ObjectInformation& _format
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
    set_output_format
    (
        size_t _index,
        const ObjectInformation& _format
    );

    void
    set_index_range_converter
    (
        size_t _output,
        const IndexRangeConverter& _value
    );

    void
    set_defined_indices_converter
    (
        size_t _output,
        const IndexCollectionConverter& _value
    );

    void
    set_wanted_indices_converter
    (
        size_t _input,
        const IndexCollectionConverter& _value
    );

private:

    SB_DECL_HIDDEN
    static
    void
    construct
    (
        AbstractFilter* _this
    );

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTFILTER_H
