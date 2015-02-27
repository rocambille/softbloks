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
#ifndef SB_ABSTRACTSINK_H
#define SB_ABSTRACTSINK_H

#include "sb-abstractblok.h"

namespace sb
{

class SB_CORE_API AbstractSink : public AbstractBlok
{

    SB_DECLARE_OBJECT(AbstractSink, "sb::AbstractSink")

public:

    class Private;

    virtual
    ~AbstractSink
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
        const ObjectInformation& format_
    );

private:

    SB_DECL_HIDDEN
    static
    void
    construct
    (
        AbstractSink* this_
    );

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTSINK_H
