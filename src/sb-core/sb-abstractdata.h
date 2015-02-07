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
#ifndef SB_ABSTRACTDATA_H
#define SB_ABSTRACTDATA_H

#include "sb-abstractobject.h"

namespace sb
{

class SB_CORE_API AbstractData : public AbstractObject
{

public:

    class Private;

    virtual
    ~AbstractData
    (
    );

protected:

    AbstractData
    (
    );

private:

    Private*
    d_ptr;

};

typedef
    std::shared_ptr<AbstractData>
    SharedData;

class SB_CORE_API DataSet : public AbstractObject
{

public:

    class Private;

    DataSet
    (
    );

    virtual
    ~DataSet
    (
    );

    IndexRange
    get_index_range
    (
    )
    const;

    IndexCollection
    get_defined_indices
    (
    )
    const;

    IndexCollection
    get_wanted_indices
    (
    )
    const;

    SharedData
    get_data
    (
        double _index
    )
    const;

    void
    set_data
    (
        double _index,
        const SharedData& _value
    );

private:

    Private*
    d_ptr;

};

typedef
    std::shared_ptr<DataSet>
    SharedDataSet;

}

#endif // SB_ABSTRACTDATA_H
