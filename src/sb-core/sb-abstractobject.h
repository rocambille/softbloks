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

#include <sb-core/sb-coredefine.h>

#include <algorithm>
#include <string>

namespace sb
{

template<typename T>
inline
bool
register_object
(
);

class AbstractObject;

template<typename T>
using Unique = std::unique_ptr<T, std::function<void(AbstractObject*)>>;

using UniqueObject = Unique<AbstractObject>;

using ObjectFactory = std::function<UniqueObject(void)>;

class SB_CORE_API AbstractObject
{

    SB_DECLARE_OBJECT(AbstractObject, "sb::AbstractObject")

public:

    template<typename T>
    struct Accessors
    {

        using Get = std::function<T(void)>;

        using Set = std::function<void(const T&)>;

        Get
        get;

        Set
        set;

    };

    struct Property
    {

        void*
        owner;

        PropertyFormat
        format;

        std::shared_ptr<void>
        accessors;

    };

    using PropertyMap = std::map<std::string, Property>;

    class Private;

    // deletion of copy-constructor. copy-constructor for a derived class
    // won't work unless it is explicitly defined and it doesn't call this
    // constructor: by default, copy is disabled
    AbstractObject
    (
        const AbstractObject& other_
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
        const AbstractObject& other_
    )
    = delete;

    ObjectFormat
    get_instance_format
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
        bool is_ready_
    );

    template<typename T>
    inline
    T
    get
    (
        const std::string& name_
    )
    const
    {
        Property wanted_property = this->properties->at(name_);

        // check type

        std::type_index wanted_result_type = typeid(T);

        if(wanted_result_type != wanted_property.format.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: property " +
                name_ +
                " registered as " +
                wanted_property.format.type.name() +
                " is accessed as " +
                wanted_result_type.name()
            );
        }

        // check mode

        if((wanted_property.format.mode & READ_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: " +
                "calling on property " +
                name_ +
                " which is write-only"
            );
        }

        // call is valid

        return std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->get();
    }

    template<typename T>
    inline
    void
    set
    (
        const std::string& name_,
        const T& value_
    )
    {
        Property wanted_property = this->properties->at(name_);

        // check type

        std::type_index wanted_argument_type = typeid(T);

        if(wanted_argument_type != wanted_property.format.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: property " +
                name_ +
                " registered as " +
                wanted_property.format.type.name() +
                " is accessed as " +
                wanted_argument_type.name()
            );
        }

        // check mode

        if((wanted_property.format.mode & WRITE_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: " +
                "calling on property " +
                name_ +
                " which is read-only"
            );
        }

        // call is valid

        std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->set(value_);
    }

    template<typename T>
    inline
    bool
    register_property
    (
        void* owner_,
        const std::string& name_,
        Mode mode_,
        const typename Accessors<T>::Get& get_,
        const typename Accessors<T>::Set& set_
    )
    {
        bool registered = false;

        if(this->properties->count(name_) == 0)
        {
            // initialize the new property

            auto accessors = std::make_shared< Accessors<T> >();
            accessors->get = get_;
            accessors->set = set_;

            Property new_property = {
                owner_,
                typeid(T),
                mode_,
                std::static_pointer_cast<void>(
                    accessors
                )
            };

            // store the property

            this->properties->emplace(name_, new_property);

            registered = true;
        }

        return registered;
    }

    bool
    unregister_property
    (
        void* owner_,
        const std::string& name_
    );

protected:

    template<typename T>
    inline
    bool
    register_property
    (
        const std::string& name_,
        Mode mode_,
        const typename Accessors<T>::Get& get_,
        const typename Accessors<T>::Set& set_
    )
    {
        return this->register_property<T>(
            this,
            name_,
            mode_,
            get_,
            set_
        );
    }

    bool
    unregister_property
    (
        const std::string& name_
    );

protected:

    static
    void
    add_type_name
    (
        AbstractObject* this_,
        std::string type_name_
    );

private:

    static
    void
    construct
    (
        AbstractObject* this_
    );

    static
    void
    init
    (
        AbstractObject* this_
    );

    static
    void
    forget
    (
        AbstractObject* this_
    );

    static
    bool
    register_object
    (
        const std::string& name_,
        const ObjectFactory& factory_
    );

    PropertyMap*
    properties;

    Private*
    d_ptr;

};

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

using SharedObject = Shared<AbstractObject>;

SB_CORE_API
SharedObject
create_shared_object
(
    const std::string& name_
);

template<typename T>
inline
std::shared_ptr<T>
create_shared
(
    const std::string& name_
)
{
    return std::static_pointer_cast<T>(
        create_shared_object(name_)
    );
}

SB_CORE_API
UniqueObject
create_unique_object
(
    const std::string& name_
);

template<typename T>
inline
std::unique_ptr<T, UniqueObject::deleter_type>
create_unique
(
    const std::string& name_
)
{
    return sb::static_pointer_cast<T>(
        create_unique_object(name_)
    );
}

template<typename T>
inline
bool
register_object
(
)
{
    auto factory = (
        []
        (
        )
        {
            auto instance = UniqueObject(
                new T,
                []
                (
                    AbstractObject* ptr_
                )
                {
                    delete ptr_;

                    AbstractObject::forget(ptr_);
                }
            );

            AbstractObject::init(instance.get());

            return instance;
        }
    );

    return AbstractObject::register_object(
        T::get_object_name(),
        factory
    );
}

SB_CORE_API
std::vector<std::string>
get_registered_object_names
(
    const ObjectFormat& filter_ = any_object_format
);

SB_CORE_API
ObjectFormat
get_object_format
(
    const std::string& name_
);

}

#endif // SB_ABSTRACTOBJECT_H
