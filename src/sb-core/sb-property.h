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

#include <sb-core/sb-propertyformat.h>

namespace sb
{

template<typename Base>
struct Property : PropertyFormat
{

    template<typename Derived, typename Type>
    using Get = std::function<Type(const Derived&)>;

    Get<Base, Any>
    get;

    template<typename Derived, typename Type>
    using Set = std::function<void(Derived&, const Type&)>;

    Set<Base, Any>
    set;

    template<typename Derived, typename Type>
    Property
    (
        const std::string& name_,
        Get<Derived, Type> get_,
        Set<Derived, Type> set_ = SB_NULLPTR
    ):
        PropertyFormat{
            typeid(Type),
            name_,
            AccessRights::NONE
        },
        get(Caller<Derived, Type>(get_)),
        set(Caller<Derived, Type>(set_))
    {
        if(get_)
        {
            bitmask(this->access_rights).set(AccessRights::READ);
        }
        if(set_)
        {
            bitmask(this->access_rights).set(AccessRights::WRITE);
        }
    }

    template<typename Derived, typename Type>
    Property
    (
        const std::string& name_,
        Type(Derived::* get_)() const,
        void(Derived::* set_)(const Type&) = SB_NULLPTR
    ):
        Property(
            name_,
            Get<Derived, Type>(
                get_
            ),
            Set<Derived, Type>(
                set_
            )
        )
    {
    }

    template<typename Derived, typename Type>
    Property
    (
        const std::string& name_,
        Set<Derived, Type> set_
    ):
        Property(
            name_,
            SB_NULLPTR,
            set_
        )
    {
    }

    template<typename Derived, typename Type>
    Property
    (
        const std::string& name_,
        void(Derived::* set_)(const Type&)
    ):
        Property(
            name_,
            Get<Derived, Type>(
                SB_NULLPTR
            ),
            Set<Derived, Type>(
                set_
            )
        )
    {
    }

private:

    template<typename Derived, typename Type>
    struct Caller
    {

        Get<Derived, Type>
        get;

        Set<Derived, Type>
        set;

        Caller
        (
            Get<Derived, Type> get_
        ):
            get(get_)
        {
        }

        Caller
        (
            Set<Derived, Type> set_
        ):
            set(set_)
        {
        }

        Any
        operator()
        (
            const Base& object_
        )
        {
            return Any(
                this->get(
                    static_cast<const Derived&>(object_)
                )
            );
        }

        void
        operator()
        (
            Base& object_,
            const Any& value_
        )
        {
            this->set(
                static_cast<Derived&>(object_),
                any_cast<Type>(value_)
            );
        }

    };

};

template<typename T>
using PropertySequence = std::vector<Property<T>>;

}

#endif // SB_PROPERTY_H
