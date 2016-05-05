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

#include <algorithm>
#include <array>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <typeindex>
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

/// Alias for a type representing a key identifier for a data.
using DataKey = double;

/// Alias for an array of 2 values representing a range for data keys.
using DataKeyRange = std::array<sb::DataKey, 2>;

/// Alias for a function that provides a mapping of a range for data keys from
/// one model to another.
using DataKeyRangeMapper = std::function<
    sb::DataKeyRange
    (
        const std::vector<sb::DataKeyRange>&
    )
>;

/// Alias for a collection of data keys.
using DataKeyCollection = std::vector<sb::DataKey>;

/// Alias for a function that provides a mapping of a collection of data keys
/// from one model to another.
using DataKeyCollectionMapper = std::function<
    sb::DataKeyCollection
    (
        const std::vector<sb::DataKeyCollection>&
    )
>;

/// \brief This enum describes a bitmask for access rights.
///
/// The applicable operators for this enum are overloaded using
/// SB_DECLARE_BITMASK_OPERATORS().
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

SB_DECLARE_BITMASK_OPERATORS(sb::AccessRights)

/// \brief The PropertyFormat structure represents the format of an
/// AbstractObject's property.
///
/// The format includes the type and the access rights of the property.
///
/// \sa \ref property-system "Softbloks's property system".
struct PropertyFormat
{

    /// This attribute holds the type of the property.
    std::type_index
    type;

    /// This attribute holds the access rights of the property.
    sb::AccessRights
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
    const sb::PropertyFormat& left_,
    const sb::PropertyFormat& right_
)
{
    return (
        left_.type == right_.type
    ) && (
        left_.access_rights == right_.access_rights
    );
}

/// Alias for a collection of properties associated to a std::string.
using PropertyFormatMap = std::map<std::string, sb::PropertyFormat>;

/// \brief The ObjectFormat structure describes the format of an
/// AbstractObject.
///
/// The format includes the type name of an object, the type names of its base
/// classes and a list of properties.
struct ObjectFormat
{

    /// This attribute holds a list of type names.
    ///
    /// The first one in the the list describes the type of the object. Next
    /// in the list are the types of the object's base class(es), the base(s)
    /// of the base(s) and so on, listing the complete inheritance tree of the
    /// object.
    ///
    /// \sa SB_DECLARE_CLASS().
    std::vector<std::string>
    type_names;

    /// This attribute holds a list of properties.
    ///
    /// \sa PropertyFormat SB_DECLARE_PROPERTIES().
    sb::PropertyFormatMap
    properties;

};

/// Constant value representing the format of an invalid object (not
/// inheriting AbstractObject).
const sb::ObjectFormat
UNDEFINED_OBJECT_FORMAT = { { } };

/// Constant value representing the format of a valid object (inheriting
/// AbstractObject).
const sb::ObjectFormat
ANY_OBJECT_FORMAT = { { "sb.AbstractObject" } };

/// Compares the object formats \a left_ and \a right_.
///
/// The comparison returns \b true if the formats hold the same type names and
/// the same properties; it returns \b false otherwise.
inline
bool
operator==
(
    const sb::ObjectFormat& left_,
    const sb::ObjectFormat& right_
)
{
    return (
        left_.type_names == right_.type_names
    ) && (
        left_.properties == right_.properties
    );
}

/// This operator returns \b true if \a to_extract_ can be extracted from
/// \a source_, i.e. if \a source_ includes all the type names and all the
/// properties holded by \a to_extract_; it returns \b false otherwise.
inline
bool
operator>>
(
    const sb::ObjectFormat& source_,
    const sb::ObjectFormat& to_extract_
)
{
    return (
        to_extract_.type_names.size() > 0
    ) && std::all_of(
        to_extract_.type_names.begin(),
        to_extract_.type_names.end(),
        [&source_]
        (
            const std::string& type_name_
        )
        {
            return std::find(
                source_.type_names.begin(),
                source_.type_names.end(),
                type_name_
            ) != source_.type_names.end();
        }
    ) && std::all_of(
        to_extract_.properties.begin(),
        to_extract_.properties.end(),
        [&source_]
        (
            const sb::PropertyFormatMap::value_type& value_to_extract_
        )
        {
            return std::find_if(
                source_.properties.begin(),
                source_.properties.end(),
                [&value_to_extract_]
                (
                    const sb::PropertyFormatMap::value_type& source_value_
                )
                {
                    return (
                        value_to_extract_.first ==
                        source_value_.first
                    ) && (
                        value_to_extract_.second.type ==
                        source_value_.second.type
                    ) && (
                        sb::bitmask(
                            value_to_extract_.second.access_rights
                        ).is_set(
                            source_value_.second.access_rights
                        )
                    );
                }
            ) != source_.properties.end();
        }
    );
}

/// This operator returns \b true if \a to_inject_ can be injected into
/// \a target_, i.e. if \a to_inject_ doesn't include a type name, nor a
/// property, which isn't holded by \a to_inject_; it returns \b false
/// otherwise.
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

/// \cond INTERNAL
template<typename T>
class Meta;
/// \endcond

/// \cond INTERNAL
template<typename T>
inline
std::vector<std::string>
get_type_names
(
)
{
    return std::vector<std::string>();
}
/// \endcond

/// Returns the type name for the type \a T.
///
/// \sa SB_DECLARE_CLASS() and get_object_format().
template<typename T>
inline
std::string
get_type_name
(
)
{
    return sb::get_type_names<T>()[0];
}

/// \cond INTERNAL
template<typename T>
inline
sb::PropertyFormatMap
get_properties
(
)
{
    return sb::get_properties<typename sb::Meta<T>::super_class>();
}
/// \endcond

}

/// This macro declares the class \a type_ as a registerable object for
/// Softbloks.
///
/// \a type_ must inherit AbstractObject or a derived class and
/// \a super_class_ must also be a declared object:
///
/// \code{cpp}
/// class Foo : public sb::AbstractObject
/// {
/// };
///
/// class Bar : public Foo
/// {
/// };
///
/// // declare the objects
///
/// SB_DECLARE_CLASS(
///     Foo,
///     "Foo",
///     sb::AbstractObject
/// )
/// SB_DECLARE_CLASS(
///     Bar,
///     "Bar",
///     Foo
/// )
/// \endcode
///
/// Once registered with the function register_object() (e.g. in a module
/// declaration), the type can be instantiated using create_unique_object() or
/// create_shared_object() with \a name_ as parameter:
///
/// \code{cpp}
/// // register the declared objects
///
/// sb::register_object<Foo>();
/// sb::register_object<Bar>();
///
/// // objects can now be instantianted with their names,
/// // either directly or by using get_type_name()
///
/// sb::create_unique_object(
///     "Foo"
/// );
/// sb::create_unique_object(
///     sb::get_type_name<Foo>()
/// );
/// \endcode
///
/// Names can also be retrieved using get_registered_object_names():
///
/// \code{cpp}
/// auto names = sb::get_registered_object_names(
///     {{
///         sb::get_type_name<Foo>() // means "Foo or a derived class"
///     }}
/// ); // contains "Foo" and "Bar"
/// \endcode
///
/// Note that SB_DECLARE_CLASS() must be called outside of any namespace.
///
/// \sa SB_DECLARE_PROPERTIES() and SB_DECLARE_MODULE().
#define SB_DECLARE_CLASS(type_, name_, super_class_)\
    namespace sb\
    {\
        template<>\
        class Meta<type_>\
        {\
            public: using super_class = super_class_;\
        };\
        template<>\
        inline\
        std::vector<std::string>\
        get_type_names<type_>\
        (\
        )\
        {\
            std::vector<std::string> type_names =\
                sb::get_type_names<super_class_>();\
            type_names.insert(type_names.begin(), name_);\
            return type_names;\
        }\
    }

/// \cond INTERNAL
#define SB_DECLARE_PROPERTIES_(type_, properties_)\
    namespace sb\
    {\
        template<>\
        inline\
        sb::PropertyFormatMap\
        get_properties<type_>\
        (\
        )\
        {\
            sb::PropertyFormatMap properties(properties_);\
            sb::PropertyFormatMap super_class_properties =\
                sb::get_properties<sb::Meta<type_>::super_class>();\
            properties.insert(\
                super_class_properties.begin(),\
                super_class_properties.end()\
            );\
            return properties;\
        }\
    }
/// \endcond

/// This macro can be used to declare a property format with \a type_ and
/// \a access_rights_ associated to \a name_.
///
/// The declared property has the type PropertyFormatMap::value_type.
///
/// \sa SB_DECLARE_PROPERTIES(), AbstractFilter::set_input_format() and
/// AbstractSink::set_input_format().
#define SB_PROPERTY(name_, type_, access_rights_)\
    {name_, {typeid(type_), access_rights_}}

/// This macro declares properties for the declared object \a type_.
///
/// Property declaration is optional and allows to expose all or part of an
/// object's properties:
///
/// \code{cpp}
/// class Foo : public sb::AbstractObject
/// {
///     Foo()
///     {
///         // create 2 properties:
///         // the first will be declared later using SB_DECLARE_PROPERTIES(),
///         // the second will be kept hidden from Softbloks
///
///         this->register_property<int>(
///             "declared_property",
///             [](){ return 0; },  // reading accessor
///             [](const int&){ }   // writing accessor
///         );
///
///         this->register_property<int>(
///             "hidden_property",
///             [](){ return 0; },  // reading accessor
///             [](const int&){ }   // writing accessor
///         );
///     }
/// };
///
/// // declare the object Foo
///
/// SB_DECLARE_CLASS(
///     Foo,
///     "Foo",
///     sb::AbstractObject
/// )
///
/// // declare its public properties
///
/// SB_DECLARE_PROPERTIES(
///     Foo,
///     SB_PROPERTY("declared_property", int, sb::AccessRights::READ_WRITE)
/// )
/// \endcode
///
/// Once registered, declared properties can be queried without instantiating
/// any object -- so without executing constructor's code:
///
/// \code{cpp}
/// sb::register_object<Foo>();
///
/// sb::get_object_format("Foo").properties; // contains "declared_property"
/// \endcode
///
/// Properties are also useful to filter registered names on a non type-based
/// criterion:
///
/// \code{cpp}
/// auto names = sb::get_registered_object_names(
///     {
///         {
///             // no type name specified:
///             // we want to filter through properties
///         },
///         {
///             SB_PROPERTY("declared_property", int, sb::AccessRights::READ_WRITE)
///         }
///     }
/// ); // contains "Foo"
///
/// names = sb::get_registered_object_names(
///     {
///         {
///             // no type name specified:
///             // we want to filter through properties
///         },
///         {
///             SB_PROPERTY("hidden_property", int, sb::AccessRights::READ_WRITE)
///         }
///     }
/// ); // DOES NOT contain "Foo" because "hidden_property" was not declared
/// \endcode
///
/// Note that SB_DECLARE_PROPERTIES() must be called outside of any namespace.
///
/// \sa SB_DECLARE_CLASS() and SB_DECLARE_MODULE().
#define SB_DECLARE_PROPERTIES(type_, ...)\
    SB_DECLARE_PROPERTIES_(\
        type_,\
        std::initializer_list<sb::PropertyFormatMap::value_type>(\
            {__VA_ARGS__}\
        )\
    )

/// This macro declares a Softbloks module.
///
/// Modules are useful to encapsulate registration of declared objects in a
/// shared library, to be dynamically loaded as a plugin in a compatible tool
/// (e.g. Softrun or Software).
///
/// Note that SB_DECLARE_MODULE() must be called outside of any namespace.
///
/// \sa SB_DECLARE_CLASS() and SB_DECLARE_PROPERTIES().
#define SB_DECLARE_MODULE(descriptor_)\
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
