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

#include <sb-global/sb-globaldefine.h>

#include <functional>
#include <type_traits>

namespace sb
{

/// \brief The BitmaskWrapper template class can be used as a reference
/// wrapper for the enumeration type \a T.
///
/// The enumeration is considered to be a bitmask type, i.e. an enumeration
/// with bitwise exclusive values. The wrapper provides an object interface
/// with convenient methods for bitmasks. Note that the enumeration  should
/// declare the necessary bitwise operators, e.g. using
/// SB_BITMASK_OPERATORS().
///
/// An appropriate bitmask wrapper can be constructed with the function
/// bitmask():
///
/// \code{cpp}
/// enum class MyBitmask
/// {
///     FOO = 1 << 0,
///     BAR = 1 << 1
/// };
///
/// SB_BITMASK_OPERATORS(MyBitmask)
///
/// ...
///
/// MyBitmask b = MyBitmask::FOO;
///
/// sb::bitmask(
///     b               // creates a wrapper holding b to call methods on it,
/// ).is_empty();       // e.g. is_empty() which here returns false
///
/// sb::bitmask(
///     b
/// ).clear(
///     MyBitmask::FOO  // let's clear a bit in the mask...
/// ).set(
///     MyBitmask::BAR  // ...and set another one
/// );
///
/// sb::bitmask(b).is_set(MyBitmask::FOO); // returns false
/// sb::bitmask(b).is_set(MyBitmask::BAR); // returns true
/// \endcode
///
/// \sa make_empty_bitmask().
template<typename T>
struct BitmaskWrapper : public std::reference_wrapper<
    // T should be an enum, disable it otherwise
    typename std::enable_if<std::is_enum<T>::value, T>::type
>
{

    /// Alias for the underlying type of \a T.
    using UnderlyingType = typename std::underlying_type<T>::type;

    /// Constructs a bitmask wrapper on the lvalue reference \a value_.
    BitmaskWrapper
    (
        T& value_
    ):
        std::reference_wrapper<T>(value_)
    {
    }

    /// Returns \b true if there is a bit set in this object, i.e. if the
    /// underlying value of this object is nonzero; returns \b false otherwise.
    explicit
    operator
    bool
    (
    )
    const
    {
        return static_cast<UnderlyingType>(
            this->get()
        ) != 0;
    }

    /// Returns \b true if there is no bits set in this object, i.e. if the
    /// underlying value of this object is zero; returns \b false otherwise.
    bool
    is_empty
    (
    )
    const
    {
        return static_cast<UnderlyingType>(
            this->get()
        ) == 0;
    }

    /// Returns \b true if all bits set in \a value_ are set in this object;
    /// returns \b false otherwise.
    ///
    /// The test is performed by evaluating the following expression:
    ///
    /// \code{cpp}
    /// ( (*this) & value_ ) == value_
    /// \endcode
    bool
    is_set
    (
        T value_
    )
    const
    {
        return ( (*this) & value_ ) == value_;
    }

    /// Sets \a value_ in this object, i.e. evaluates the following
    /// expression:
    ///
    /// \code{cpp}
    /// (*this) |= value_;
    /// \endcode
    ///
    /// The function returns a reference to this object.
    BitmaskWrapper&
    set
    (
        T value_
    )
    {
        (*this) |= value_;

        return (*this);
    }

    /// Clears \a value_ in this object, i.e. evaluates the following
    /// expression:
    ///
    /// \code{cpp}
    /// (*this) &= ~value_;
    /// \endcode
    ///
    /// The function returns a reference to this object.
    BitmaskWrapper&
    clear
    (
        T value_
    )
    {
        (*this) &= ~value_;

        return (*this);
    }

};

/// Returns a bitmask wrapper of the appropriate type holding a reference to
/// \a value_.
///
/// Note that the type \a T must be an enumeration.
///
/// \sa make_empty_bitmask().
template<typename T>
inline
BitmaskWrapper<T>
bitmask
(
    T& value_
)
{
    return BitmaskWrapper<T>(value_);
}

/// Returns an empty bitmask of the appropriate type, i.e. the value 0
/// statically cast into the type \a T.
///
/// Note that the type \a T must be an enumeration.
///
/// \sa bitmask().
template<
    typename T,
    // T should be an enum, disable it otherwise
    typename = typename std::enable_if<std::is_enum<T>::value>::type
>
SB_CONSTEXPR_FUNCTION
T
make_empty_bitmask
(
)
{
    return static_cast<T>(0);
}

}

/// Overloads the bitwise operators for the bitmask type \a bitmask_
/// (&, |, ^, ~, &=, |= and ^=).
///
/// \sa BitmaskWrapper.
#define SB_BITMASK_OPERATORS(bitmask_)\
SB_STATIC_ASSERT_MSG(\
    std::is_enum<bitmask_>::value,\
    "declared bitmask operators on non enum type"\
);\
SB_CONSTEXPR_FUNCTION \
bitmask_ \
operator&\
(\
    bitmask_ left_,\
    bitmask_ right_\
)\
{\
    return static_cast<bitmask_>(\
        static_cast<std::underlying_type<bitmask_>::type>(left_) &\
        static_cast<std::underlying_type<bitmask_>::type>(right_)\
    );\
}\
\
SB_CONSTEXPR_FUNCTION \
bitmask_ \
operator|\
(\
    bitmask_ left_,\
    bitmask_ right_\
)\
{\
    return static_cast<bitmask_>(\
        static_cast<std::underlying_type<bitmask_>::type>(left_) |\
        static_cast<std::underlying_type<bitmask_>::type>(right_)\
    );\
}\
\
SB_CONSTEXPR_FUNCTION \
bitmask_ \
operator^\
(\
    bitmask_ left_,\
    bitmask_ right_\
)\
{\
    return static_cast<bitmask_>(\
        static_cast<std::underlying_type<bitmask_>::type>(left_) ^\
        static_cast<std::underlying_type<bitmask_>::type>(right_)\
    );\
}\
\
SB_CONSTEXPR_FUNCTION \
bitmask_ \
operator~\
(\
    bitmask_ value_\
)\
{\
    return static_cast<bitmask_>(\
        ~static_cast<std::underlying_type<bitmask_>::type>(value_)\
    );\
}\
\
inline \
bitmask_& \
operator&=\
(\
    bitmask_& left_,\
    bitmask_ right_\
)\
{\
    left_ = left_ & right_;\
\
    return left_;\
}\
\
inline \
bitmask_& \
operator|=\
(\
    bitmask_& left_,\
    bitmask_ right_\
)\
{\
    left_ = left_ | right_;\
\
    return left_;\
}\
\
inline \
bitmask_& \
operator^=\
(\
    bitmask_& left_,\
    bitmask_ right_\
)\
{\
    left_ = left_ ^ right_;\
\
    return left_;\
}

#endif // SB_BITMASK_H
