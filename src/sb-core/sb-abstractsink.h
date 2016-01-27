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
#ifndef SB_ABSTRACTSINK_H
#define SB_ABSTRACTSINK_H

#include <sb-core/sb-abstractblok.h>

namespace sb
{

SB_CLASS(SB_CORE_API, AbstractSink, "sb.AbstractSink", AbstractBlok)
{

public:

    class Private;

    AbstractSink
    (
    );

    virtual
    ~AbstractSink
    (
    );

    SharedDataSet
    lock_input
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

    void
    set_wanted_indices
    (
        size_t input_,
        const IndexCollection& value_
    );

protected:

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

private:

    Private*
    d_ptr;

};

using UniqueSink = Unique<AbstractSink>;

static
UniqueSink
(&create_unique_sink)
(
    const std::string& name_
) = create_unique<AbstractSink>;

}

#endif // SB_ABSTRACTSINK_H
