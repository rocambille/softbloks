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
#ifndef SB_BITMASK_H
#define SB_BITMASK_H

#include <sb-global/sb-utility.h>

#include <functional>
#include <type_traits>

namespace sb
{

/// Returns an empty bitmask of the appropriate type, i.e. the value 0
/// statically cast into the type \a Bitmask.
///
/// Note that the type \a Bitmask must be an enumeration.
template<
    typename Bitmask,
    // Bitmask should be an enum, disable it otherwise
    typename = typename std::enable_if_t<std::is_enum<Bitmask>::value>
>
constexpr
Bitmask
make_empty_bitmask
(
)
{
    return static_cast<Bitmask>(0);
}

/// Returns \b true if there is no bits set in \a bitmask_, i.e. if the
/// underlying value of this object is zero; returns \b false otherwise.
///
/// Note that the type \a Bitmask must be an enumeration.
template<
    typename Bitmask,
    // Bitmask should be an enum, disable it otherwise
    typename = typename std::enable_if_t<std::is_enum<Bitmask>::value>
>
constexpr
bool
is_empty_bitmask
(
    const Bitmask& bitmask_
)
{
    return static_cast<typename std::underlying_type_t<Bitmask>>(
        bitmask_
    ) == 0;
}

/// Returns \b true if all bits set in \a bit_ are set in the calling bitmask;
/// returns \b false otherwise.
///
/// The test is performed by evaluating the following expression:
///
/// \code{cpp}
/// (bitmask & bit_) == bit_
/// \endcode
///
/// Note that the type \a Bitmask must be an enumeration.
template<
    typename Bitmask,
    // Bitmask should be an enum, disable it otherwise
    typename = typename std::enable_if_t<std::is_enum<Bitmask>::value>
>
constexpr
auto
is_set
(
    const Bitmask& bit_
)
{
    return [&](const Bitmask& bitmask_)
    {
        return (bitmask_ & bit_) == bit_;
    };
}

/// Sets \a bit_ in the calling bitmask, i.e. evaluates the following
/// expression:
///
/// \code{cpp}
/// bitmask |= bit_;
/// \endcode
///
/// The function returns a reference to the calling bitmask.
///
/// Note that the type \a Bitmask must be an enumeration.
template<
    typename Bitmask,
    // Bitmask should be an enum, disable it otherwise
    typename = typename std::enable_if_t<std::is_enum<Bitmask>::value>
>
constexpr
auto
set
(
    const Bitmask& bit_
)
{
    return [&](Bitmask& bitmask_) -> Bitmask&
    {
        bitmask_ |= bit_;
        return bitmask_;
    };
}

/// Clears \a bit_ in the calling bitmask, i.e. evaluates the following
/// expression:
///
/// \code{cpp}
/// bitmask &= ~bit_;
/// \endcode
///
/// The function returns a reference to the calling bitmask.
///
/// Note that the type \a Bitmask must be an enumeration.
template<
    typename Bitmask,
    // Bitmask should be an enum, disable it otherwise
    typename = typename std::enable_if_t<std::is_enum<Bitmask>::value>
>
constexpr
auto
clear
(
    const Bitmask& bit_
)
{
    return [&](Bitmask& bitmask_) -> Bitmask&
    {
        bitmask_ &= (~bit_);
        return bitmask_;
    };
}

}

/// Overloads the bitwise operators for the bitmask type \a Bitmask_
/// (&, |, ^, ~, &=, |= and ^=).
///
/// \sa BitmaskWrapper.
#define SB_BITMASK_OPERATORS(Bitmask_)\
static_assert(\
    std::is_enum<Bitmask_>::value,\
    "declared bitmask operators on non enum type"\
);\
constexpr \
Bitmask_ \
operator&\
(\
    const Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    return static_cast<Bitmask_>(\
        static_cast<std::underlying_type_t<Bitmask_>>(left_) &\
        static_cast<std::underlying_type_t<Bitmask_>>(right_)\
    );\
}\
\
constexpr \
Bitmask_ \
operator|\
(\
    const Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    return static_cast<Bitmask_>(\
        static_cast<std::underlying_type_t<Bitmask_>>(left_) |\
        static_cast<std::underlying_type_t<Bitmask_>>(right_)\
    );\
}\
\
constexpr \
Bitmask_ \
operator^\
(\
    const Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    return static_cast<Bitmask_>(\
        static_cast<std::underlying_type_t<Bitmask_>>(left_) ^\
        static_cast<std::underlying_type_t<Bitmask_>>(right_)\
    );\
}\
\
constexpr \
Bitmask_ \
operator~\
(\
    const Bitmask_& value_\
)\
{\
    return static_cast<Bitmask_>(\
        ~static_cast<std::underlying_type_t<Bitmask_>>(value_)\
    );\
}\
\
inline \
Bitmask_& \
operator&=\
(\
    Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    left_ = left_ & right_;\
\
    return left_;\
}\
\
inline \
Bitmask_& \
operator|=\
(\
    Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    left_ = left_ | right_;\
\
    return left_;\
}\
\
inline \
Bitmask_& \
operator^=\
(\
    Bitmask_& left_,\
    const Bitmask_& right_\
)\
{\
    left_ = left_ ^ right_;\
\
    return left_;\
}

namespace sb
{

/// \brief This enum describes a bitmask for access rights.
///
/// The applicable operators for this enum are overloaded using
/// SB_BITMASK_OPERATORS().
///
/// \sa BitmaskWrapper.
enum class AccessRights
{

// empty bitmask

    NONE        = 0,            ///< No access permissions.

// bitmask values

    READ        = 1 << 0,       ///< Read permission.
    WRITE       = 1 << 1,       ///< Write permission.

// bitwise combinations

    READ_WRITE  = READ | WRITE  ///< Read/write permissions.

};

SB_BITMASK_OPERATORS(AccessRights)

}

#endif // SB_BITMASK_H
