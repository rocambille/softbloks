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
    ObjectFactoryMap;

typedef
    std::map<std::string, ObjectInformation>
    ObjectInformationMap;

namespace Global
{

ObjectFactoryMap
object_factory_map;

ObjectInformationMap
object_information_map;

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
        const ObjectFactoryMap::value_type& _value
    )
    {
        return _value.first;
    }

    inline
    ObjectFactory
    factory
    (
        const ObjectFactoryMap::value_type& _value
    )
    {
        return _value.second;
    }

    inline
    std::string
    name
    (
        const ObjectInformationMap::value_type& _value
    )
    {
        return _value.first;
    }

    inline
    ObjectInformation
    information
    (
        const ObjectInformationMap::value_type& _value
    )
    {
        return _value.second;
    }

}

}

using namespace sb;

std::vector<std::string>
sb::get_registered_objects
(
)
{
    std::vector<std::string> registered_objects;
    registered_objects.reserve(
        Global::object_factory_map.size()
    );

    for(auto object_factory : Global::object_factory_map)
    {
        registered_objects.push_back(
            Unmapper::name(object_factory)
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

    auto object_factory =
        Global::object_factory_map.find(_name);

    if(object_factory != Global::object_factory_map.end())
    {
        instance = Unmapper::factory(*object_factory)();
    }

    return instance;
}

ObjectInformation
sb::get_object_information
(
    const std::string& _name
)
{
    return Global::object_information_map.at(_name);
}

AbstractObject::~AbstractObject
(
)
{
    delete d_ptr;

    delete this->properties;
}

ObjectInformation
AbstractObject::get_instance_information
(
)
const
{
    PropertyInformationMap property_information;

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
AbstractObject::add_type_name
(
    AbstractObject* _this,
    std::string _type_name
)
{
    auto& type_names = AbstractObject::Private::from(
        _this
    )->type_names;

    type_names.insert(
        type_names.begin(),
        _type_name
    );
}

void
AbstractObject::construct
(
    AbstractObject* _this
)
{
    _this->properties = new std::map<std::string, Property>;

    _this->d_ptr = new Private(_this);
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

    if(Global::object_factory_map.count(_name) == 0)
    {
        Global::object_factory_map.emplace(
            _name,
            _factory
        );

        auto instance = _factory();

        Global::object_information_map.emplace(
            _name,
            instance->get_instance_information()
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
