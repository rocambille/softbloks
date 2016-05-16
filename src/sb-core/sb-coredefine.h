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
#ifndef SB_COREDEFINE_H
#define SB_COREDEFINE_H

#include <sb-global/sb-global.h>
#include <sb-global/sb-bitmask.h>

#include <limits>
#include <memory>
#include <string>
#include <vector>

/// \cond INTERNAL
#ifdef sb_core_EXPORTS
#define SB_CORE_API SB_DECL_EXPORT
#else
#define SB_CORE_API SB_DECL_IMPORT
#endif
/// \endcond

namespace sb
{

class AbstractObject;

/// Template alias for a managed pointer uniquely owned.
///
/// The pointer type \a T must be compatible with a deleter of AbstractObject.
template<typename T>
using Unique = std::unique_ptr<T, std::function<void(AbstractObject*)>>;

/// Template alias for a managed pointer with shared ownership.
template<typename T>
using Shared = std::shared_ptr<T>;

/// Template alias for a weakly managed pointer.
template<typename T>
using Weak = std::weak_ptr<T>;

/// Alias for a type representing the size of an object.
using Size = size_t;

/// Constant symbolizing the maximum finite value for the type Size.
const Size MAX_SIZE = std::numeric_limits<Size>::max();

/// Alias for a type representing a position in a container.
using Index = Size;

/// Alias for a sequence container of strings.
using StringSequence = std::vector<std::string>;

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

/// Returns a std::unique_ptr object move-constructed from \a unique_pointer_.
///
/// The standard move-cast-constructor of std::unique_ptr needs T* to be
/// implicitly convertible to U*. This function explicitly perfoms a static
/// cast on the stored pointer, so the constraints on the types are reduced.
template<typename U, typename T, typename D>
std::unique_ptr<U, D>
static_move_cast
(
    std::unique_ptr<T, D>&& unique_pointer_
)
{
    U* stored_pointer = static_cast<U*>(
        unique_pointer_.get()
    );

    std::unique_ptr<U, D> moved_pointer(
        stored_pointer,
        std::move(unique_pointer_.get_deleter())
    );

    unique_pointer_.release();

    return moved_pointer;
}

/// Returns the type name for the type \a T.
///
/// \sa SB_NAME() and get_object_format().
template<typename T>
inline
std::string
get_type_name
(
)
{
    return T::get_type_names()[0];
}

}

/// This macro declares a Softbloks module.
///
/// Modules are useful to encapsulate registration of declared objects in a
/// shared library, to be dynamically loaded as a plugin in a compatible tool
/// (e.g. Softrun or Software).
///
/// Note that SB_MODULE() must be called outside of any namespace.
#define SB_MODULE(descriptor_)\
    namespace sb\
    {\
        const char descriptor[] = "{" #descriptor_ "}";\
    }\
    extern "C"\
    SB_DECL_EXPORT\
    const char*\
    sb_get_module_descriptor\
    (\
    )\
    {\
        return sb::descriptor;\
    }\
    extern "C"\
    SB_DECL_EXPORT\
    void\
    sb_main\
    (\
    )

#endif // SB_COREDEFINE_H
