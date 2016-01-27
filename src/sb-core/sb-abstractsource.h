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

SB_CLASS(SB_CORE_API, AbstractSource, "sb.AbstractSource", AbstractBlok)
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
        size_t index_
    )
    const;

protected:

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
        const ObjectFormat& format
    );

    void
    set_index_range
    (
        size_t output_,
        const IndexRange& value_
    );

    void
    set_defined_indices
    (
        size_t output_,
        const IndexCollection& value_
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

#endif // SB_ABSTRACTSOURCE_H
