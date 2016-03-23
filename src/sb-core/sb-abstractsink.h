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

class SB_CORE_API AbstractSink : public sb::AbstractBlok
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
        sb::Index index_
    )
    const;

    bool
    set_input
    (
        sb::Index index_,
        const SharedDataSet& value_
    );

    void
    set_wanted_data_keys
    (
        sb::Index input_,
        const DataKeyCollection& value_
    );

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

SB_DECLARE_CLASS(
    sb::AbstractSink,
    "sb.AbstractSink",
    sb::AbstractBlok
)

#endif // SB_ABSTRACTSINK_H
