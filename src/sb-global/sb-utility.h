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
#ifndef SB_UTILITY_H
#define SB_UTILITY_H

#include <sb-global/sb-globaldefine.h>

#include <type_traits>

namespace sb
{

template<typename T, typename F>
auto
operator->*
(
    T* t_,
    const F& f_
) -> std::enable_if_t<std::is_void<decltype(f_(*t_))>::value>
{
    f_(*t_);
}

template<typename T, typename F>
auto
operator->*
(
    T* t_,
    const F& f_
) -> std::enable_if_t<!std::is_void<decltype(f_(*t_))>::value, decltype(f_(*t_))>
{
    return f_(*t_);
}

template<typename T, typename F>
auto
operator->*
(
    T& t_,
    const F& f_
) -> std::enable_if_t<std::is_void<decltype(f_(t_))>::value>
{
    f_(t_);
}

template<typename T, typename F>
auto
operator->*
(
    T& t_,
    const F& f_
) -> std::enable_if_t<!std::is_void<decltype(f_(t_))>::value, decltype(f_(t_))>
{
    return f_(t_);
}

}

#endif // SB_UTILITY_H
