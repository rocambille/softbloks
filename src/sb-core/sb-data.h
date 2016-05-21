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
#ifndef SB_DATA_H
#define SB_DATA_H

#include <sb-core/sb-abstractdata.h>

#include <sstream>

namespace sb
{

template<typename Type>
class Data : public AbstractData
{

    SB_NAME(
        static_cast<std::ostringstream&>(
            std::ostringstream() <<
                "sb.Data<" <<
                typeid(Type).hash_code() <<
                ">"
        ).str()
    )

    SB_PROPERTIES({
        "value",
        &Data::get_value,
        &Data::set_value
    })

public:

    Type
    get_value
    (
    )
    const
    {
        return this->value;
    }

    void
    set_value
    (
        const Type& value_
    )
    {
        this->value = value_;
    }

private:

    Type
    value;

};

template<typename T>
bool
register_data
(
)
{
    return register_object<Data<T>>();
}

}

#endif // SB_DATA_H
