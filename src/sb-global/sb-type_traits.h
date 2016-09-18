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
#ifndef SB_TYPE_TRAITS_H
#define SB_TYPE_TRAITS_H

#include <sb-global/sb-globaldefine.h>

#include <type_traits>
#include <functional>

namespace sb
{

template<typename... > struct type_list {};

template <typename T, typename... Types>
struct get_types : get_types<
    std::conditional_t<
        std::is_same<T, typename T::Self>::value,
        typename T::Base,
        typename T::Self
    >,
    Types..., T
> {};

template<typename... Types>
struct get_types<void, Types...>
{
    using types = type_list<Types...>;
};

template<typename T>
using get_types_t = typename get_types<T>::types;

template<typename T>
struct tag
{
    using type = T;
};

template<typename T> struct for_each {};

template<typename... Types>
struct for_each<type_list<Types...>> {
    template<typename F>
    for_each(F f) {
        using swallow = int[];
        (void)swallow{ 0, (f(tag<Types>{}), 0)... };
    }
};

template<typename Object, typename Value>
using get_t = std::function<Value(const Object&)>;

template<typename Object, typename Value>
using set_t = std::function<void(Object&, const Value&)>;

template<typename T>
struct gettor_traits : std::false_type {};

template<typename Object, typename Value>
struct gettor_traits< get_t<Object, Value> > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

template<typename Object, typename Value>
struct gettor_traits< Value(Object::*)() const > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

template<typename Object, typename Value>
struct gettor_traits< Value(*)(const Object&) > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

template<class T>
struct settor_traits : std::false_type {};

template<typename Object, typename Value>
struct settor_traits< set_t<Object, Value> > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

template<typename Object, typename Value>
struct settor_traits< void(Object::*)(const Value&) > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

template<typename Object, typename Value>
struct settor_traits< void(*)(Object&, const Value&) > : std::true_type
{
    using object_type = Object;
    using value_type = Value;
};

}

#endif // SB_TYPE_TRAITS_H
