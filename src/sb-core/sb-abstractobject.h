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

struct PropertyFormat
{

    std::type_index
    type;

    sb::Mode
    mode;

};

typedef
    std::map<std::string, PropertyFormat>
    PropertyFormatMap;

namespace Unmapper
{

    inline
    std::string
    name
    (
        const PropertyFormatMap::value_type& value_
    )
    {
        return value_.first;
    }

    inline
    PropertyFormat
    format
    (
        const PropertyFormatMap::value_type& value_
    )
    {
        return value_.second;
    }

}

struct ObjectFormat
{

    std::vector<std::string>
    type_names;

    PropertyFormatMap
    properties;

};

const ObjectFormat
undefined_format = { { "sb::UndefinedObject" } };

const ObjectFormat
any_format = { { "sb::AbstractObject" } };

class AbstractObject;

typedef
    std::shared_ptr<AbstractObject>
    SharedObject;

typedef
    std::function<SharedObject(void)>
    ObjectFactory;

template<typename T>
inline
bool
register_object
(
)
{
    return AbstractObject::register_object(
        T::get_object_name(),
        []
        (
        )
        {
            auto instance = SharedObject(
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
}

SB_CORE_API
std::vector<std::string>
get_registered_object_names
(
    const ObjectFormat& filter_ = any_format
);

SB_CORE_API
SharedObject
create_object
(
    const std::string& name_
);

template<typename T>
inline
std::shared_ptr<T>
create
(
    const std::string& name_
)
{
    return std::static_pointer_cast<T>(
        create_object(name_)
    );
}

SB_CORE_API
ObjectFormat
get_object_format
(
    const std::string& name_
);

class SB_CORE_API AbstractObject
{

    SB_DECLARE_OBJECT(AbstractObject, "sb::AbstractObject")

public:

    template<typename T>
    struct Accessors
    {

        typedef
            std::function<T(void)>
            Get;

        typedef
            std::function<void(const T&)>
            Set;

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

    typedef
        std::map<std::string, Property>
        PropertyMap;

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

    SB_DECL_HIDDEN
    static
    void
    add_type_name
    (
        AbstractObject* this_,
        std::string type_name_
    );

private:

    SB_DECL_HIDDEN
    static
    void
    construct
    (
        AbstractObject* this_
    );

    SB_DECL_HIDDEN
    static
    void
    init
    (
        AbstractObject* this_
    );

    SB_DECL_HIDDEN
    static
    void
    forget
    (
        AbstractObject* this_
    );

    SB_DECL_HIDDEN
    static
    bool
    register_object
    (
        const std::string& name_,
        const ObjectFactory& factory_
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

}

inline
bool
operator>>
(
    const sb::ObjectFormat& from_,
    const sb::ObjectFormat& to_extract_
)
{
    return std::all_of(
        to_extract_.type_names.begin(),
        to_extract_.type_names.end(),
        [&from_]
        (
            const std::string& type_name_
        )
        {
            return std::find(
                from_.type_names.begin(),
                from_.type_names.end(),
                type_name_
            ) != from_.type_names.end();
        }
    ) && std::all_of(
        to_extract_.properties.begin(),
        to_extract_.properties.end(),
        [&from_]
        (
            const sb::PropertyFormatMap::value_type& value_to_extract_
        )
        {
            return std::find_if(
                from_.properties.begin(),
                from_.properties.end(),
                [&value_to_extract_]
                (
                    const sb::PropertyFormatMap::value_type& from_value_
                )
                {
                    return (
                        sb::Unmapper::name(
                            value_to_extract_
                        ) ==
                        sb::Unmapper::name(
                            from_value_
                        )
                    ) && (
                        sb::Unmapper::format(
                            value_to_extract_
                        ).type ==
                        sb::Unmapper::format(
                            from_value_
                        ).type
                    ) && (
                        (
                            sb::Unmapper::format(
                                value_to_extract_
                            ).mode &
                            sb::Unmapper::format(
                                from_value_
                            ).mode
                        ) ==
                        sb::Unmapper::format(value_to_extract_).mode
                    );
                }
            ) != from_.properties.end();
        }
    );
}

inline
bool
operator<<
(
    const sb::ObjectFormat& target_,
    const sb::ObjectFormat& to_inject_
)
{
    return to_inject_ >> target_;
}

#endif // SB_ABSTRACTOBJECT_H
