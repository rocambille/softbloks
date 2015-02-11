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

namespace sb
{

typedef
    std::map<std::string, ObjectFactory>
    ObjectNameToFactoryMap;

namespace Global
{

ObjectNameToFactoryMap
registered_objects;

}

namespace Unmapper
{

inline
std::string
name
(
    const AbstractObject::PropertyMap::value_type& _value
)
{
    return _value.first;
}

inline
AbstractObject::Property
data
(
    const AbstractObject::PropertyMap::value_type& _value
)
{
    return _value.second;
}

inline
std::string
name
(
    const ObjectNameToFactoryMap::value_type& _value
)
{
    return _value.first;
}

inline
ObjectFactory
factory
(
    const ObjectNameToFactoryMap::value_type& _value
)
{
    return _value.second;
}

}

}

using namespace sb;

AbstractObject::~AbstractObject
(
)
{
    delete d_ptr;

    delete this->properties;
}

ObjectInformation
AbstractObject::get_information
(
)
const
{
    std::map<std::string, PropertyInformation> property_information;

    for(auto property : *(this->properties))
    {
        property_information.emplace(
            Unmapper::name(property),
            Unmapper::data(property).information
        );
    }

    ObjectInformation information = {
        d_ptr->type_names,
        property_information
    };

    return information;
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

bool
AbstractObject::unregister_property
(
    void* _owner,
    const std::string& _name
)
{
    bool unregistered = false;

    // check if _name is known and if _owner matches

    auto find_result = this->properties->find(_name);

    if(
        find_result != this->properties->end() &&
        Unmapper::data(*find_result).owner == _owner
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

    AbstractObject::construct(this, "sb::AbstractObject");
}

bool
AbstractObject::unregister_property
(
    const std::string& _name
)
{
    return this->unregister_property(
        this,
        _name
    );
}

void
AbstractObject::construct
(
    AbstractObject* _this,
    std::string _type_name
)
{
    AbstractObject::Private::from(
        _this
    )->type_names.push_back(
        _type_name
    );
}

void
AbstractObject::init
(
    AbstractObject* _this
)
{
}

void
AbstractObject::forget
(
    AbstractObject* _this
)
{
}

bool
AbstractObject::register_object
(
    const std::string& _name,
    const ObjectFactory& _factory
)
{
    bool registered = false;

    if(Global::registered_objects.count(_name) == 0)
    {
        Global::registered_objects.emplace(
            _name,
            _factory
        );

        registered = true;
    }

    return registered;
}

AbstractObject::Private::Private
(
    AbstractObject* _q
):
    q_ptr   (_q),
    is_ready(false)
{
}

AbstractObject::Private*
AbstractObject::Private::from
(
    const AbstractObject* _q
)
{
    return _q->d_ptr;
}

std::vector<std::string>
sb::get_registered_objects
(
)
{
    std::vector<std::string> registered_objects;
    registered_objects.reserve(
        Global::registered_objects.size()
    );

    for(auto registration : Global::registered_objects)
    {
        registered_objects.push_back(
            Unmapper::name(registration)
        );
    }

    return registered_objects;
}

SharedObject
sb::create_object
(
    const std::string& _name
)
{
    SharedObject instance;

    auto registration =
        Global::registered_objects.find(_name);

    if(registration != Global::registered_objects.end())
    {
        instance = Unmapper::factory(*registration)();
    }

    return instance;
}
