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
#ifndef SB_ABSTRACTOBJECT_H
#define SB_ABSTRACTOBJECT_H

#include "sb-coredefine.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <typeindex>

namespace sb
{

struct PropertyInformation
{
    std::type_index
    type;

    sb::Mode
    mode;
};

struct ObjectInformation
{
    std::string
    object_name;

    std::map<std::string, PropertyInformation>
    properties;
};

class AbstractObject;

typedef
    std::shared_ptr<AbstractObject>
    SharedObject;

typedef
    std::function<SharedObject(void)>
    ObjectFactory;

class SB_CORE_API AbstractObject
{

public:

    template<typename T>
    struct Accessors
    {
        typedef
            std::function<T(void)>
            Get;

        Get
        get;

        typedef
            std::function<void(const T&)>
            Set;

        Set
        set;
    };

    struct Property
    {
        void*
        owner;

        PropertyInformation
        information;

        std::shared_ptr<void>
        accessors;
    };

    typedef
        std::map<std::string, Property>
        PropertyMap;

    class Private;

    // deletion of copy-constructor. copy-constructor for a derived class
    // won't work unless it is explicitly defined and it doesn't call this
    // constructor: by default, copy is disabled
    AbstractObject
    (
        const AbstractObject& _other
    )
    = delete;

    virtual
    ~AbstractObject
    (
    );

    // deletion of operator=. operator= for a derived class won't work unless
    // it is explicitly defined and it doesn't call this operator: by default,
    // copy is disabled
    AbstractObject&
    operator=
    (
        const AbstractObject& _other
    )
    = delete;

    ObjectInformation
    get_information
    (
    )
    const;

    bool
    is_ready
    (
    )
    const;

    void
    set_ready
    (
        bool _is_ready
    );

    template<typename T>
    T
    get
    (
        const std::string& _name
    )
    const
    {
        Property wanted_property = this->properties->at(_name);

        // check type

        std::type_index wanted_result_type = typeid(T);

        if(wanted_result_type != wanted_property.information.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: property " +
                _name +
                " registered as " +
                wanted_property.information.type.name() +
                " is accessed as " +
                wanted_result_type.name()
            );
        }

        // check mode

        if((wanted_property.information.mode & READ_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: " +
                "calling on property " +
                _name +
                " which is write-only"
            );
        }

        // call is valid

        return std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->get();
    }

    template<typename T>
    void
    set
    (
        const std::string& _name,
        const T& _value
    )
    {
        Property wanted_property = this->properties->at(_name);

        // check type

        std::type_index wanted_argument_type = typeid(T);

        if(wanted_argument_type != wanted_property.information.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: property " +
                _name +
                " registered as " +
                wanted_property.information.type.name() +
                " is accessed as " +
                wanted_argument_type.name()
            );
        }

        // check mode

        if((wanted_property.information.mode & WRITE_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: " +
                "calling on property " +
                _name +
                " which is read-only"
            );
        }

        // call is valid

        std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->set(_value);
    }

    template<typename T>
    bool
    register_property
    (
        void* _owner,
        const std::string& _name,
        Mode _mode,
        const typename Accessors<T>::Get& _get,
        const typename Accessors<T>::Set& _set
    )
    {
        bool registered = false;

        if(this->properties->count(_name) == 0)
        {
            // initialize the new property

            auto accessors = std::make_shared< Accessors<T> >();
            accessors->get = _get;
            accessors->set = _set;

            Property new_property = {
                _owner,
                typeid(T),
                _mode,
                std::static_pointer_cast<void>(
                    accessors
                )
            };

            // store the property

            this->properties->emplace(_name, new_property);

            registered = true;
        }

        return registered;
    }

    bool
    unregister_property
    (
        void* _owner,
        const std::string& _name
    );

protected:

    AbstractObject
    (
    );

    template<typename T>
    bool
    register_property
    (
        const std::string& _name,
        Mode _mode,
        const typename Accessors<T>::Get& _get,
        const typename Accessors<T>::Set& _set
    )
    {
        return this->register_property<T>(
            this,
            _name,
            _mode,
            _get,
            _set
        );
    }

    bool
    unregister_property
    (
        const std::string& _name
    );

private:

    void
    init
    (
        std::string _object_name
    );

    static
    bool
    register_object
    (
        const std::string& _name,
        const ObjectFactory& _factory
    );

    template<typename T>
    friend
    bool
    register_object
    (
    );

    PropertyMap*
    properties;

    Private*
    d_ptr;

};

template<typename T>
bool
register_object
(
)
{
    return AbstractObject::register_object(
        T::get_name(),
        []
        (
        )
        {
            auto instance = SharedObject(
                new T,
                []
                (
                    T* _ptr
                )
                {
                    delete _ptr;
                }
            );

            instance->init(T::get_name());

            return instance;
        }
    );
}

SB_CORE_API
std::vector<std::string>
get_registered_objects
(
);

SB_CORE_API
SharedObject
create_object
(
    const std::string& _name
);

template<typename T>
std::shared_ptr<T>
create
(
    const std::string& _name
)
{
    return std::static_pointer_cast<T>(
        create_object(_name)
    );
}

}

#endif // SB_ABSTRACTOBJECT_H
