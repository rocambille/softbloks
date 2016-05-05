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
#ifndef SB_DATASET_H
#define SB_DATASET_H

#include <sb-core/sb-abstractobject.h>

#include <sb-core/sb-abstractdata.h>

namespace sb
{

class SB_CORE_API DataSet : public sb::AbstractObject
{

public:

    class Private;

    /// Constructs a data set.
    DataSet
    (
    );

    /// Destroys this object.
    virtual
    ~DataSet
    (
    );

    DataKeyRange
    get_data_key_range
    (
    )
    const;

    DataKeyCollection
    get_defined_data_keys
    (
    )
    const;

    DataKeyCollection
    get_wanted_data_keys
    (
    )
    const;

    SharedData
    get_data
    (
        DataKey data_key_
    )
    const;

    void
    set_data
    (
        DataKey data_key_,
        const SharedData& value_
    );

private:

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

/// Alias for a managed data set with shared ownership.
using SharedDataSet = Shared<DataSet>;

/// Alias for create_shared<DataSet>().
static
SharedDataSet
(&create_shared_data_set)
(
    const std::string& name_
) = create_shared<DataSet>;

}

/// \cond INTERNAL
SB_DECLARE_CLASS(
    sb::DataSet,
    "sb.DataSet",
    sb::AbstractObject
)
/// \endcond

#endif // SB_DATASET_H
