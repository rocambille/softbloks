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

    Private
    (
        AbstractBlok* q_ptr_
    );

    void
    set_input_count
    (
        Size value_
    );

    void
    set_input_count
    (
        Size minimum_,
        Size maximum_
    );

    void
    set_input_format
    (
        Size index_,
        const ObjectFormat& format_
    );

    void
    set_output_count
    (
        Size value_
    );

    void
    set_output_count
    (
        Size minimum_,
        Size maximum_
    );

    void
    update_outputs_data_key_range
    (
    );

    void
    update_outputs_defined_data_keys
    (
    );

    void
    update_inputs_wanted_data_keys
    (
    );

    SharedDataSet
    lock_input
    (
        Index index_
    )
    const;

    bool
    set_input
    (
        Index index_,
        const SharedDataSet& value_
    );

    void
    unlink_input
    (
        Index index_
    );

    static
    Private*
    from
    (
        const AbstractBlok* this_
    );

    static
    Private*
    from
    (
        const UniqueBlok& this_
    );

public:

    AbstractBlok*
    q_ptr;

    Size
    minimum_input_count;

    Size
    maximum_input_count;

    std::vector<WeakDataSet>
    inputs;

    std::vector<ObjectFormat>
    inputs_format;

    std::vector<DataKeyCollectionMapper>
    wanted_data_keys_mappers;

    Size
    minimum_output_count;

    Size
    maximum_output_count;

    std::vector<SharedDataSet>
    outputs;

    std::vector<DataKeyRangeMapper>
    data_key_range_mappers;

    std::vector<DataKeyCollectionMapper>
    defined_data_keys_mappers;

    UniqueExecutive
    executive;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
