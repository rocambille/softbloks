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
#ifndef SB_PROPERTY_H
#define SB_PROPERTY_H

#include <sb-global/sb-type_traits.h>
#include <sb-global/sb-bitmask.h>

#include <string>
#include <typeindex>
#include <vector>

namespace sb
{

/// \brief The PropertyFormat structure represents the format of an
/// AbstractObject's property.
///
/// The format includes the type and the access rights of the property.
///
/// \sa \ref property-system "Softbloks's property system".
struct PropertyFormat
{

    std::type_index
    type;

    std::string
    name;

    AccessRights
    access_rights;

};

/// Compares the property formats \a left_ and \a right_.
///
/// The comparison returns \b true if the described properties have the same
/// types and the same access rights; it returns \b false otherwise.
inline
bool
operator==
(
    const PropertyFormat& left_,
    const PropertyFormat& right_
)
{
    return (
        left_.type == right_.type
    ) && (
        left_.name == right_.name
    ) && (
        left_.access_rights == right_.access_rights
    );
}

inline
bool
operator<
(
    const PropertyFormat& left_,
    const PropertyFormat& right_
)
{
    return left_.name < right_.name;
}

template<typename T>
PropertyFormat
make_property_format
(
    const std::string& name_,
    AccessRights access_rights_
)
{
    return {
        typeid(T),
        name_,
        access_rights_
    };
}

using PropertyFormatSequence = std::vector<PropertyFormat>;

template<typename Base>
class Property : public PropertyFormat
{

public:

    template<
        typename Gettor,
        typename Settor,
        std::enable_if_t<
            gettor_traits<Gettor>::value &&
            settor_traits<Settor>::value &&
            std::is_same<
                typename gettor_traits<Gettor>::object_type,
                typename settor_traits<Settor>::object_type
            >::value &&
            std::is_same<
                typename gettor_traits<Gettor>::value_type,
                typename settor_traits<Settor>::value_type
            >::value &&
            std::is_base_of<
                Base, typename gettor_traits<Gettor>::object_type
            >::value,
            int
        > = 0,
        typename Object = typename gettor_traits<Gettor>::object_type,
        typename Value = typename gettor_traits<Gettor>::value_type
    >
    Property
    (
        const std::string& name_,
        Gettor gettor_,
        Settor settor_
    ):
        PropertyFormat{
            typeid(Value),
            name_,
            AccessRights::NONE
        },
        gettor{Caller<Object, Value>{get_t<Object, Value>{gettor_}}},
        settor{Caller<Object, Value>{set_t<Object, Value>{settor_}}}
    {
        if(gettor_)
        {
            bitmask(this->access_rights).set(AccessRights::READ);
        }
        if(settor_)
        {
            bitmask(this->access_rights).set(AccessRights::WRITE);
        }
    }

    template<
        typename Gettor,
        std::enable_if_t<
            gettor_traits<Gettor>::value &&
            std::is_base_of<
                Base,
                typename gettor_traits<Gettor>::object_type
            >::value,
            int
        > = 0,
        typename Object = typename gettor_traits<Gettor>::object_type,
        typename Value = typename gettor_traits<Gettor>::value_type
    >
    Property
    (
        const std::string& name_,
        Gettor gettor_
    ):
        Property{
            name_,
            gettor_,
            set_t<Object, Value>{}
        }
    {
    }

    template<
        typename Settor,
        std::enable_if_t<
            settor_traits<Settor>::value &&
            std::is_base_of<
                Base,
                typename settor_traits<Settor>::object_type
            >::value,
            int
        > = 0,
        typename Object = typename settor_traits<Settor>::object_type,
        typename Value = typename settor_traits<Settor>::value_type
    >
    Property
    (
        const std::string& name_,
        Settor settor_
    ):
        Property{
            name_,
            get_t<Object, Value>{},
            settor_
        }
    {
    }

private:

    template<typename Object, typename Value>
    struct Caller
    {

        get_t<Object, Value>
        gettor;

        set_t<Object, Value>
        settor;

        Caller
        (
            get_t<Object, Value> gettor_
        ):
            gettor{gettor_}
        {
        }

        Caller
        (
            set_t<Object, Value> settor_
        ):
            settor{settor_}
        {
        }

        Any
        operator()
        (
            const Base& object_
        )
        {
            return Any{
                this->gettor(
                    static_cast<const Object&>(object_)
                )
            };
        }

        void
        operator()
        (
            Base& object_,
            const Any& value_
        )
        {
            this->settor(
                static_cast<Object&>(object_),
                any_cast<Value>(value_)
            );
        }

    };

private:

    get_t<Base, Any>
    gettor;

    set_t<Base, Any>
    settor;

    template<typename U, typename T>
    friend
    get_t<T, U>
    get
    (
        const Property<T>& p_
    );

    template<typename U, typename T>
    friend
    std::function<void(T&)>
    set
    (
        Property<T>& p_,
        const U& u_
    );

};

template<typename T, typename F>
auto
operator->*
(
    T* t_,
    F f_
) -> std::enable_if_t<std::is_void<decltype(f_(*t_))>::value>
{
    f_(*t_);
}

template<typename T, typename F>
auto
operator->*
(
    T* t_,
    F f_
) -> std::enable_if_t<!std::is_void<decltype(f_(*t_))>::value, decltype(f_(*t_))>
{
    return f_(*t_);
}

template<typename U, typename T>
get_t<T, U>
get
(
    const Property<T>& p_
)
{
    if(
        !bitmask(
            p_.access_rights
        ).is_set(
            AccessRights::READ
        )
    )
    {
        throw std::invalid_argument(
            std::string() +
            "sb::get: " +
            "calling on property " +
            p_.name +
            " which has no read access"
        );
    }

    return [&](const T& t_){ return any_cast<U>(p_.gettor(t_)); };
}

template<typename U, typename T>
std::function<void(T&)>
set
(
    Property<T>& p_, const U& u_
)
{
    if(
        !bitmask(
            p_.access_rights
        ).is_set(
            AccessRights::WRITE
        )
    )
    {
        throw std::invalid_argument(
            std::string() +
            "sb::get: " +
            "calling on property " +
            p_.name +
            " which has no write access"
        );
    }

    return [&](T& t_){ p_.settor(t_, Any{u_}); };
}

template<typename T>
using PropertySequence = std::vector<Property<T>>;

}

#endif // SB_PROPERTY_H
