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

using NameToObjectFactoryMap = std::map<std::string, ObjectFactory>;

using NameToObjectFormatMap = std::map<std::string, ObjectFormat>;

namespace Global
{

NameToObjectFactoryMap
object_factories;

NameToObjectFormatMap
object_formats;

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
        const NameToObjectFactoryMap::value_type& value_
    )
    {
        return value_.second;
    }

    inline
    ObjectFormat
    format
    (
        const NameToObjectFormatMap::value_type& value_
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
    this->d_ptr = new Private;
}

AbstractObject::~AbstractObject
(
)
{
    delete this->d_ptr;
}

ObjectFormat
AbstractObject::get_format
(
)
const
{
    return Global::object_formats.at(
        d_ptr->type_names[0]
    );
}

ObjectProperty
AbstractObject::get_property
(
    const std::string& name_
)
const
{
    return d_ptr->properties.at(name_);
}

void
AbstractObject::init
(
    AbstractObject* this_,
    const StringSequence& type_names_,
    const ObjectPropertySequence& properties_
)
{
    auto d_ptr = AbstractObject::Private::from(
        this_
    );
    
    d_ptr->type_names = type_names_;

    for(auto property : properties_)
    {
        if(
            ! d_ptr->properties.emplace(
                property.name,
                property
            ).second
        )
        {
            // insertion failed, throw an exception
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractObject::init: " +
                "failed to insert property " +
                property.name +
                "; a property with that name already exists"
            );
        }
    }

    this_->init();
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

    if(Global::object_factories.count(name) == 0)
    {
        Global::object_factories.emplace(
            name,
            factory_
        );

        Global::object_formats.emplace(
            name,
            format_
        );

        registered = true;
    }

    return registered;
}

SharedObject
sb::create_shared_object
(
    const std::string& name_
)
{
    SharedObject instance;

    auto object_factory =
        Global::object_factories.find(name_);

    if(object_factory != Global::object_factories.end())
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
        Global::object_factories.find(name_);

    if(object_factory != Global::object_factories.end())
    {
        instance = Unmapper::factory(*object_factory)();
    }

    return instance;
}

StringSequence
sb::get_registered_object_names
(
    const ObjectFormat& filter_
)
{
    StringSequence registered_objects;
    registered_objects.reserve(
        Global::object_factories.size()
    );

    for(auto object : Global::object_formats)
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
        Global::object_formats.find(name_);

    if(mapped_format != Global::object_formats.end())
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
    Global::object_factories.clear();
    Global::object_formats.clear();
}
