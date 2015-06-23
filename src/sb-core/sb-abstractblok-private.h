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

#include <sb-core/sb-abstractblok.h>

#include <sb-core/sb-abstractdata.h>
#include <sb-core/sb-dataset-private.h>

namespace sb
{

class SB_DECL_HIDDEN AbstractBlok::Private
{

public:

    AbstractBlok::Private
    (
        AbstractBlok* q_ptr_
    );

    void
    set_input_count
    (
        size_t value_
    );

    void
    set_input_count
    (
        size_t minimum_,
        size_t maximum_
    );

    void
    set_input_format
    (
        size_t index_,
        const ObjectFormat& format_
    );

    void
    set_output_count
    (
        size_t value_
    );

    void
    set_output_count
    (
        size_t minimum_,
        size_t maximum_
    );

    void
    set_output_format
    (
        size_t index_,
        const ObjectFormat& format_
    );

    void
    update_outputs_index_range
    (
    );

    void
    update_outputs_defined_indices
    (
    );

    void
    update_inputs_wanted_indices
    (
    );

    SharedDataSet
    get_input
    (
        size_t index_
    )
    const;

    bool
    set_input
    (
        size_t index_,
        const SharedDataSet& value_
    );

    SharedDataSet
    get_output
    (
        size_t index_
    )
    const;

    static
    AbstractBlok::Private*
    from
    (
        const AbstractBlok* this_
    );

    static
    AbstractBlok::Private*
    from
    (
        const UniqueBlok& this_
    );

public:

    AbstractBlok*
    q_ptr;

    size_t
    minimum_input_count;

    size_t
    maximum_input_count;

    std::vector<WeakDataSet>
    inputs;

    std::vector<ObjectFormat>
    inputs_format;

    std::vector<IndexCollectionConverter>
    wanted_indices_converters;

    size_t
    minimum_output_count;

    size_t
    maximum_output_count;

    std::vector<SharedDataSet>
    outputs;

    std::vector<IndexRangeConverter>
    index_range_converters;

    std::vector<IndexCollectionConverter>
    defined_indices_converters;

    UniqueExecutive
    executive;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
