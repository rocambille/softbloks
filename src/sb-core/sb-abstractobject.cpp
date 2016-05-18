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
#include <sb-core/sb-abstractobject.h>

#include <sb-core/sb-abstractobject-private.h>

namespace sb
{

using ObjectFactoryMap = std::map<std::string, ObjectFactory>;

using ObjectFormatMap = std::map<std::string, ObjectFormat>;

namespace Global
{

ObjectFactoryMap
object_factory_map;

ObjectFormatMap
object_format_map;

}

namespace Unmapper
{

    template<typename T>
    inline
    std::string
    name
    (
        const T& value_
    )
    {
        return value_.first;
    }

    inline
    ObjectFactory
    factory
    (
        const ObjectFactoryMap::value_type& value_
    )
    {
        return value_.second;
    }

    inline
    ObjectFormat
    format
    (
        const ObjectFormatMap::value_type& value_
    )
    {
        return value_.second;
    }

}

}

using namespace sb;

AbstractObject::AbstractObject
(
)
{
    this->properties = new std::map<std::string, Property>;

    this->d_ptr = new Private(this);
}

AbstractObject::~AbstractObject
(
)
{
    delete d_ptr;

    delete this->properties;
}

ObjectFormat
AbstractObject::get_instance_format
(
)
const
{
    return Global::object_format_map.at(
        d_ptr->type_names[0]
    );
}

void
AbstractObject::init
(
    AbstractObject* this_,
    const StringSequence& type_names_,
    const PropertySequence& properties_
)
{
    AbstractObject::Private::from(
        this_
    )->type_names = type_names_;

    for(auto property : properties_)
    {
        if(
            ! this_->properties->emplace(
                property.name,
                property
            ).second
        )
        {
            // insertion failed, throw an exception
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::init: " +
                "failed to insert property " +
                property.name +
                "; a property with that name already exists"
            );
        }
    }

    this_->init();
}

void
AbstractObject::forget
(
    AbstractObject* this_
)
{
}

bool
AbstractObject::register_object
(
    const ObjectFormat& format_,
    const ObjectFactory& factory_
)
{
    bool registered = false;

    std::string name = format_.type_names[0];

    if(Global::object_factory_map.count(name) == 0)
    {
        Global::object_factory_map.emplace(
            name,
            factory_
        );

        Global::object_format_map.emplace(
            name,
            format_
        );

        registered = true;
    }

    return registered;
}

AbstractObject::Private::Private
(
    AbstractObject* q_ptr_
):
    q_ptr   (q_ptr_)
{
}

AbstractObject::Private*
AbstractObject::Private::from
(
    const AbstractObject* this_
)
{
    return this_->d_ptr;
}

AbstractObject::Private*
AbstractObject::Private::from
(
    const SharedObject& this_
)
{
    return this_->d_ptr;
}

SharedObject
sb::create_shared_object
(
    const std::string& name_
)
{
    SharedObject instance;

    auto object_factory =
        Global::object_factory_map.find(name_);

    if(object_factory != Global::object_factory_map.end())
    {
        instance = Unmapper::factory(*object_factory)();
    }

    return instance;
}

UniqueObject
sb::create_unique_object
(
    const std::string& name_
)
{
    UniqueObject instance;

    auto object_factory =
        Global::object_factory_map.find(name_);

    if(object_factory != Global::object_factory_map.end())
    {
        instance = Unmapper::factory(*object_factory)();
    }

    return instance;
}

std::vector<std::string>
sb::get_registered_object_names
(
    const ObjectFormat& filter_
)
{
    std::vector<std::string> registered_objects;
    registered_objects.reserve(
        Global::object_factory_map.size()
    );

    for(auto object : Global::object_format_map)
    {
        if(Unmapper::format(object).includes(filter_))
        {
            registered_objects.push_back(
                Unmapper::name(object)
            );
        }
    }

    return registered_objects;
}

ObjectFormat
sb::get_object_format
(
    const std::string& name_
)
{
    auto object_format = UNDEFINED_OBJECT_FORMAT;

    auto mapped_format =
        Global::object_format_map.find(name_);

    if(mapped_format != Global::object_format_map.end())
    {
        object_format = Unmapper::format(*mapped_format);
    }

    return object_format;
}

void
sb::unregister_all_objects
(
)
{
    Global::object_factory_map.clear();
    Global::object_format_map.clear();
}
