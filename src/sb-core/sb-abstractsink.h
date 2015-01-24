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

public:

    class Private;

    virtual
    ~AbstractSink
    (
    );

    SharedDataSet
    get_input
    (
        size_t _index
    )
    const;

    void
    set_input
    (
        size_t _index,
        const SharedDataSet& _value
    );

    void
    set_wanted_steps
    (
        size_t _input,
        const StepList& _value
    );

protected:

    AbstractSink
    (
    );

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

private:

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTSINK_H
