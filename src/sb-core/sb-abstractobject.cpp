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
#include "sb-abstractobject.h"

#include "sb-abstractobject-private.h"

using namespace sb;

AbstractObject::~AbstractObject
(
)
{
    delete d_ptr;

    delete this->properties;
}

bool
AbstractObject::is_ready
(
)
const
{
    return d_ptr->is_ready;
}

void
AbstractObject::set_ready
(
    bool _is_ready
)
{
    d_ptr->is_ready = _is_ready;
}

std::list<std::string>
AbstractObject::get_property_names
(
)
const
{
    std::list<std::string> names;

    for(auto property : *(this->properties))
    {
        names.push_back(
            property.first
        );
    }

    return names;
}

std::type_index
AbstractObject::get_property_type
(
    const std::string& _name
)
const
{
    return this->properties->at(_name).type;
}

sb::Mode
AbstractObject::get_property_mode
(
    const std::string& _name
)
const
{
    return this->properties->at(_name).mode;
}

bool
AbstractObject::unregister_property_for
(
    void* _caller,
    const std::string& _name
)
{
    bool unregistered = false;

    // check if _name is known and if _caller matches

    auto find_result = this->properties->find(_name);

    if(
        find_result != this->properties->end() &&
        find_result->second.register_caller == _caller
    )
    {
        // erase property

        this->properties->erase(find_result);

        unregistered = true;
    }

    return unregistered;
}

AbstractObject::AbstractObject
(
)
{
    this->properties = new std::map<std::string, Property>;

    d_ptr = new Private(this);
}

bool
AbstractObject::unregister_property
(
    const std::string& _name
)
{
    return this->unregister_property_for(
        this,
        _name
    );
}

AbstractObject::Private::Private
(
    AbstractObject* _q
):
    q_ptr   (_q),
    is_ready(false)
{
}
