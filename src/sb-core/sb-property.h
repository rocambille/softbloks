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

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace sb
{

template<
    typename T,
    typename U,
    typename = typename std::enable_if<std::is_base_of<AbstractObject, T>::value>::type
>
using Get = std::function<U(const T&)>;

template<
    typename T,
    typename U,
    typename = typename std::enable_if<std::is_base_of<AbstractObject, T>::value>::type
>
using Set = std::function<void(T&, const U&)>;

/// \cond INTERNAL
template<typename T, typename U>
struct Accessors
{

    Get<T, U>
    get;

    Set<T, U>
    set;

};
/// \endcond

struct Property : PropertyFormat
{

    /// \cond INTERNAL
    std::shared_ptr<void>
    accessors;
    /// \endcond

    template<typename T, typename U>
    Property
    (
        const std::string& name_,
        Get<T, U> get_,
        Set<T, U> set_ = SB_NULLPTR
    ):
        PropertyFormat{
            typeid(U),
            name_,
            AccessRights::NONE
        }
    {
        std::shared_ptr< Accessors<T, U> > get_set =
            std::make_shared< Accessors<T, U> >();

        get_set->get = get_;
        get_set->set = set_;

        if(get_)
        {
            bitmask(this->access_rights).set(AccessRights::READ);
        }
        if(set_)
        {
            bitmask(this->access_rights).set(AccessRights::WRITE);
        }

        this->accessors = std::static_pointer_cast<void>(get_set);
    }

    template<typename T, typename U>
    Property
    (
        const std::string& name_,
        U(T::* get_)() const,
        void(T::* set_)(const U&) = SB_NULLPTR
    ):
        Property(
            name_,
            Get<T, U>(
                get_
            ),
            Set<T, U>(
                set_
            )
        )
    {
    }

    template<typename T, typename U>
    Property
    (
        const std::string& name_,
        Set<T, U> set_
    ):
        Property(
            name_,
            SB_NULLPTR,
            set_
        )
    {
    }

    template<typename T, typename U>
    Property
    (
        const std::string& name_,
        void(T::* set_)(const U&)
    ):
        Property(
            name_,
            Get<T, U>(
                SB_NULLPTR
            ),
            Set<T, U>(
                set_
            )
        )
    {
    }

};

/// \cond INTERNAL
using PropertySequence = std::vector<Property>;

using PropertyMap = std::map<std::string, Property>;
/// \endcond

}

#endif // SB_PROPERTY_H
