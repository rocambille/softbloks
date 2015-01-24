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
#include <list>
#include <map>
#include <memory>
#include <string>
#include <typeindex>

namespace sb
{

class SB_CORE_API AbstractObject
{

public:

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

    std::list<std::string>
    get_property_names
    (
    )
    const;

    std::type_index
    get_property_type
    (
        const std::string& _name
    )
    const;

    sb::Mode
    get_property_mode
    (
        const std::string& _name
    )
    const;

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

        if(wanted_result_type != wanted_property.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: property " +
                _name +
                " registered as " +
                wanted_property.type.name() +
                " is accessed as " +
                wanted_result_type.name()
            );
        }

        // check mode

        if((wanted_property.mode & READ_ONLY) == 0)
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

        return reinterpret_cast<PropertyValues<T>*>(
            wanted_property.values.get()
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

        if(wanted_argument_type != wanted_property.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: property " +
                _name +
                " registered as " +
                wanted_property.type.name() +
                " is accessed as " +
                wanted_argument_type.name()
            );
        }

        // check mode

        if((wanted_property.mode & WRITE_ONLY) == 0)
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

        reinterpret_cast<PropertyValues<T>*>(
            wanted_property.values.get()
        )->set(_value);
    }

    template<typename T>
    bool
    register_property_for
    (
        void* _caller,
        const std::string& _name,
        Mode _mode,
        const std::function<T(void)>& _get,
        const std::function<void(const T&)>& _set
    )
    {
        bool registered = false;

        if(this->properties.count(_name) == 0)
        {
            // initialize the new property

            PropertyValues<T>* new_property_values = new PropertyValues<T>;
            new_property_values->get = _get;
            new_property_values->set = _set;

            Property new_property = {
                _caller,
                std::type_index(typeid(T)),
                _mode,
                std::shared_ptr<void>(
                    reinterpret_cast<void*>(
                        new_property_values
                    ),
                    []
                    (
                        void* _ptr
                    )
                    {
                        delete reinterpret_cast<PropertyValues<T>*>(
                            _ptr
                        );
                    }
                )
            };

            // store the property

            this->properties->emplace(_name, new_property);

            registered = true;
        }

        return registered;
    }

    bool
    unregister_property_for
    (
        void* _caller,
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
        const std::function<T(void)>& _get,
        const std::function<void(const T&)>& _set
    )
    {
        return this->register_property_for(
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

    template<typename T>
    struct PropertyValues
    {
        std::function<T(void)>          get;
        std::function<void(const T&)>   set;
    };

    struct Property
    {
        void*                           register_caller;
        std::type_index                 type;
        sb::Mode                        mode;
        std::shared_ptr<void>           values;
    };

    std::map<std::string, Property>*    properties;

    Private*
    d_ptr;

};

}

#endif // SB_ABSTRACTOBJECT_H
