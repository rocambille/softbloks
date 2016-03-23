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
#ifndef SB_ABSTRACTFILTER_H
#define SB_ABSTRACTFILTER_H

#include <sb-core/sb-abstractblok.h>

namespace sb
{

class SB_CORE_API AbstractFilter : public sb::AbstractBlok
{

public:

    class Private;

    AbstractFilter
    (
    );

    virtual
    ~AbstractFilter
    (
    );

    SharedDataSet
    lock_input
    (
        sb::Index index_
    )
    const;

    bool
    set_input
    (
        sb::Index index_,
        const SharedDataSet& value_
    );

    SharedDataSet
    get_output
    (
        sb::Index index_
    )
    const;

protected:

    void
    set_input_count
    (
        sb::Size value_
    );

    void
    set_input_count
    (
        sb::Size minimum_,
        sb::Size maximum_
    );

    void
    set_input_format
    (
        sb::Index index_,
        const ObjectFormat& format_
    );

    void
    set_output_count
    (
        sb::Size value_
    );

    void
    set_output_count
    (
        sb::Size minimum_,
        sb::Size maximum_
    );

    void
    set_output_format
    (
        sb::Index index_,
        const ObjectFormat& format_
    );

    void
    set_data_key_range_mapper
    (
        sb::Index output_,
        const DataKeyRangeMapper& value_
    );

    void
    set_defined_data_keys_mapper
    (
        sb::Index output_,
        const DataKeyCollectionMapper& value_
    );

    void
    set_wanted_data_keys_mapper
    (
        sb::Index input_,
        const DataKeyCollectionMapper& value_
    );

private:

    Private*
    d_ptr;

};

using UniqueFilter = Unique<AbstractFilter>;

static
UniqueFilter
(&create_unique_filter)
(
    const std::string& name_
) = create_unique<AbstractFilter>;

}

SB_DECLARE_CLASS(
    sb::AbstractFilter,
    "sb.AbstractFilter",
    sb::AbstractBlok
)

#endif // SB_ABSTRACTFILTER_H
