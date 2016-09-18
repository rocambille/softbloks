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
#ifndef SB_META_H
#define SB_META_H

#include <sb-global/sb-type_traits.h>

#include <vector>

namespace sb
{

template<typename T>
struct meta_return
{
    using type = std::vector<T>;
};

template<typename T, typename Alloc>
struct meta_return<std::vector<T, Alloc>>
{
    using type = std::vector<T, Alloc>;
};

template<typename T>
using meta_return_t = typename meta_return<T>::type;

template<typename T, typename U>
std::vector<T>& operator<<(std::vector<T>& container_, U&& value_)
{
    container_.insert(container_.end(), value_);
    return container_;
}

template<typename T, typename U, typename Alloc>
std::vector<T>& operator<<(std::vector<T>& container_, std::vector<U, Alloc>&& values_)
{
    container_.insert(container_.end(), values_.begin(), values_.end());
    return container_;
}

}

#define SB_META(function_, member_)\
    template<typename T>\
    meta_return_t<decltype(T::member_())> function_()\
    {\
        using data_type = decltype(T::member_());\
        using F = data_type(*)();\
        meta_return_t<data_type> data;\
\
        F previous = nullptr;\
        for_each<get_types_t<T>>{\
            [&](auto t)\
            {\
                using B = typename decltype(t)::type;\
                F current = B::member_;\
                if(current != previous)\
                {\
                    data << current();\
                    previous = current;\
                }\
            }\
        };\
\
        return data;\
    }

#define SB_ROOT(object_)\
public:\
    using Base = void;\
    using Self = object_;

#define SB_SELF(object_)\
public:\
    using Base = Self;\
    using Self = object_;

#endif // SB_META_H
