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
#ifndef SB_ABSTRACTSOURCE_H
#define SB_ABSTRACTSOURCE_H

#include <sb-core/sb-abstractblok.h>

namespace sb
{

class SB_CORE_API AbstractSource : public sb::AbstractBlok
{

public:

    class Private;

    AbstractSource
    (
    );

    virtual
    ~AbstractSource
    (
    );

    SharedDataSet
    get_output
    (
        sb::Index index_
    )
    const;

protected:

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
        const ObjectFormat& format
    );

    void
    set_data_key_range
    (
        sb::Index output_,
        const DataKeyRange& value_
    );

    void
    set_defined_data_keys
    (
        sb::Index output_,
        const DataKeyCollection& value_
    );

private:

    Private*
    d_ptr;

};

using UniqueSource = Unique<AbstractSource>;

static
UniqueSource
(&create_unique_source)
(
    const std::string& name_
) = create_unique<AbstractSource>;

}

SB_DECLARE_CLASS(
    sb::AbstractSource,
    "sb.AbstractSource",
    sb::AbstractBlok
)

#endif // SB_ABSTRACTSOURCE_H
