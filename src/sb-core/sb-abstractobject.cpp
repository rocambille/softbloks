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

using ObjectFactory = std::function<UniqueObject(void)>;

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
    PropertyFormatMap property_format;

    for(auto property : *(this->properties))
    {
        property_format.emplace(
            property.first,
            property.second.format
        );
    }

    ObjectFormat format = {
        d_ptr->type_names,
        property_format
    };

    return format;
}

bool
AbstractObject::unregister_property
(
    void* owner_,
    const std::string& name_
)
{
    bool unregistered = false;

    // check if name_ is known and if owner_ matches

    auto find_result = this->properties->find(name_);

    if(
        find_result != this->properties->end() &&
        find_result->second.owner == owner_
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
    const std::string& name_
)
{
    return this->unregister_property(
        this,
        name_
    );
}

void
AbstractObject::set_type_names
(
    AbstractObject* this_,
    std::vector<std::string> names_
)
{
    AbstractObject::Private::from(
        this_
    )->type_names = names_;
}

void
AbstractObject::init
(
    AbstractObject* this_
)
{
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
        if(Unmapper::format(object) >> filter_)
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
