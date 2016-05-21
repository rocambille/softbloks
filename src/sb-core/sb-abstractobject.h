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
#ifndef SB_ABSTRACTOBJECT_H
#define SB_ABSTRACTOBJECT_H

#include <sb-core/sb-objectformat.h>

#include <functional>
#include <map>
#include <type_traits>

namespace sb
{

using ObjectFactory = std::function<Unique<AbstractObject>(void)>;

/// \brief The AbstractObject class is the base class for all Softbloks
/// objects.
///
/// type names, properties.
///
/// \anchor property-system
/// ### The Property System ###
///
/// Properties can be associated to a Softbloks object in class declaration:
///
/// \code{cpp}
/// class Foo : public sb::AbstractObject
/// {
///
///     SB_PROPERTIES({
///         "my_property",
///         &Foo::get_int,
///         &Foo::set_int
///     })
///
/// public:
///
///     Foo()
///     {
///         // a property should not be accessed in the constructor,
///         // see init() below
///     }
///
///     virtual
///     void
///     init
///     override
///     {
///         // the property can be accessed from here using get() or set()
///
///         this->get<int>("my_property");      // executes Foo::get_int()
///         this->set<int>("my_property", 42);  // executes Foo::set_int()
///     }
///
///     int get_int() const { return 42; }
///
///     void set_int(const int& /*value_*/) { }
///
/// };
/// \endcode
///
/// Through
/// [std::function](http://www.cplusplus.com/reference/functional/function/),
/// an accessor may be a function, a function pointer, a function object, a
/// lambda expression or a pointer to member -- bound to an instance using
/// [std::bind()](http://www.cplusplus.com/reference/functional/bind/).
class SB_CORE_API AbstractObject
{

public:

    using Self = AbstractObject;

    class Private;

    // deletion of copy-constructor. copy-constructor for a derived class
    // won't work unless it is explicitly defined and it doesn't call this
    // constructor: by default, copy is disabled
    AbstractObject
    (
        const AbstractObject& other_
    )
    SB_DELETED_FUNCTION;

    /// Constructs a Softbloks object.
    AbstractObject
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractObject
    (
    );

    // deletion of operator=. operator= for a derived class won't work unless
    // it is explicitly defined and it doesn't call this operator: by default,
    // copy is disabled
    AbstractObject&
    operator=
    (
        const AbstractObject& other_
    )
    SB_DELETED_FUNCTION;

    virtual
    void
    init
    (
    )
    {
    }

    /// Returns the format of this object.
    ObjectFormat
    get_format
    (
    )
    const;

    /// Returns current value of the property \a name_.
    ///
    /// An exception is raised if this object has no properties called
    /// \a name_, if the property was declared with a type different from \a T
    /// or if the property was declared in write-only mode.
    ///
    /// \sa set() and \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    T
    get
    (
        const std::string& name_
    )
    const
    {
        return any_cast<T>(
            this->get(name_)
        );
    }

    /// Sets the property \a name_ to \a value_.
    ///
    /// An exception is raised if this object has no properties called
    /// \a name_, if the property was declared with a type different from \a T
    /// or if the property was declared in read-only mode.
    ///
    /// \sa get() and \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    void
    set
    (
        const std::string& name_,
        const T& value_
    )
    {
        this->set(name_, Any(value_));
    }

    static
    StringSequence
    get_type_names
    (
    )
    {
        return { "sb.AbstractObject" };
    }

    static
    PropertySequence<AbstractObject>
    get_properties
    (
    )
    {
        return { };
    }

    /// \cond INTERNAL
    template<typename T>
    static
    void
    on_creation
    (
        AbstractObject* this_
    )
    {
        AbstractObject::init(
            this_,
            T::get_type_names(),
            T::get_properties()
        );
    }
    /// \endcond

    /// \cond INTERNAL
    template<typename T>
    static
    void
    on_destruction
    (
        AbstractObject* /*this_*/
    )
    {
    }
    /// \endcond

private:

    /// \cond INTERNAL
    Any
    get
    (
        const std::string& name_
    )
    const;
    /// \endcond

    /// \cond INTERNAL
    void
    set
    (
        const std::string& name_,
        const Any& value_
    );
    /// \endcond

    /// \cond INTERNAL
    static
    void
    init
    (
        AbstractObject* this_,
        const StringSequence& type_names_,
        const PropertySequence<AbstractObject>& properties_
    );
    /// \endcond

    /// \cond INTERNAL
    template<typename T>
    friend
    inline
    bool
    register_object
    (
    );

    static
    bool
    register_object
    (
        const ObjectFormat& format_,
        const ObjectFactory& factory_
    );
    /// \endcond

    /// \cond INTERNAL
    Private*
    d_ptr;
    /// \endcond

};

using ObjectProperty = Property<AbstractObject>;
using ObjectPropertySequence = PropertySequence<AbstractObject>;

/// Constant value representing the format of a valid object (inheriting
/// AbstractObject).
const ObjectFormat
ANY_OBJECT_FORMAT = {
    AbstractObject::get_type_names(),
    AbstractObject::get_properties()
};

/// Alias for a managed object with shared ownership.
using SharedObject = Shared<AbstractObject>;

/// Returns a managed pointer to an instance of the object designated by
/// \a name_.
///
/// If \a T wasn't registered using register_object() before calling this
/// function, the returned pointer will be empty (pointing to \b nullptr).
///
/// Valid names can be retrieved with get_registered_object_names().
///
/// The returned pointer has shared ownership.
///
/// \sa create_shared() and create_unique_object().
SB_CORE_API
SharedObject
create_shared_object
(
    const std::string& name_
);

/// Constructs an instance of the object designated by \a name_ and returns it
/// as a managed pointer casted statically to the type \a T.
///
/// If \a T wasn't registered using register_object() before calling this
/// function, the returned pointer will be empty (pointing to \b nullptr).
///
/// Valid names can be retrieved with get_registered_object_names().
///
/// The returned pointer has shared ownership.
///
/// \sa create_shared_object().
template<typename T>
inline
Shared<T>
create_shared
(
    const std::string& name_
)
{
    return std::static_pointer_cast<T>(
        create_shared_object(name_)
    );
}

/// Alias for a managed object uniquely owned.
using UniqueObject = Unique<AbstractObject>;

/// Returns a managed pointer to an instance of the object designated by
/// \a name_.
///
/// If \a T wasn't registered using register_object() before calling this
/// function, the returned pointer will be empty (pointing to \b nullptr).
///
/// Valid names can be retrieved with get_registered_object_names().
///
/// The returned pointer is uniquely owned.
///
/// \sa create_unique() and create_shared_object().
SB_CORE_API
UniqueObject
create_unique_object
(
    const std::string& name_
);

/// Constructs an instance of the object designated by \a name_ and returns it
/// as a managed pointer casted statically to the type \a T.
///
/// If \a T wasn't registered using register_object() before calling this
/// function, the returned pointer will be empty (pointing to \b nullptr).
///
/// Valid names can be retrieved with get_registered_object_names().
///
/// The returned pointer is uniquely owned.
///
/// \sa create_unique_object().
template<typename T>
inline
Unique<T>
create_unique
(
    const std::string& name_
)
{
    return static_move_cast<T>(
        create_unique_object(name_)
    );
}

/// Registers the type \a T as an instantiable object using
/// create_shared_object() or one of the associated functions.
///
/// The type \a T should be given a unique name with SB_NAME().
///
/// The function returns \b true if there was no previously registered type
/// with the same name as \a T; it returns \b false otherwise.
///
/// \sa unregister_all_objects().
template<typename T>
inline
bool
register_object
(
)
{
    ObjectFactory factory = (
        []
        (
        )
        {
            Unique<T> instance = Unique<T>(
                new T,
                []
                (
                    AbstractObject* ptr_
                )
                {
                    T::template on_destruction<T>(ptr_);

                    delete ptr_;
                }
            );

            T::template on_creation<T>(instance.get());

            return static_move_cast<AbstractObject>(
                std::move(instance)
            );
        }
    );

    return AbstractObject::register_object(
        {
            T::get_type_names(),
            T::get_properties()
        },
        factory
    );
}

/// Returns a list of the names of all the objects previously registered with
/// register_object().
///
/// The names can be filtered by type names and/or by properties using the
/// \a filter_ argument.
///
/// \sa SB_NAME() and SB_PROPERTIES().
SB_CORE_API
StringSequence
get_registered_object_names
(
    const ObjectFormat& filter_ = ANY_OBJECT_FORMAT
);

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

/// Returns the format of the object identified by \a name_.
///
/// The object must have been previously registered using register_object().
/// The function returns UNDEFINED_OBJECT_FORMAT otherwise.
///
/// \sa \ref property-system "Softbloks's property system".
SB_CORE_API
ObjectFormat
get_object_format
(
    const std::string& name_
);

template<typename T>
inline
ObjectFormat
get_object_format
(
)
{
    return get_object_format(
        get_type_name<T>()
    );
}

/// Unregisters all the objects previously registered using register_object().
///
/// You shouldn't normally call this function, unless you are integrating
/// Softbloks in your own application and your application dynamically load
/// modules using functions like LoadLibrary() or dlopen(): in that case you
/// should call this function before unloading the modules.
SB_CORE_API
void
unregister_all_objects
(
);

}

#define SB_SELF(type_)\
    public:\
        using Base = Self;\
        using Self = type_;

#define SB_NAME(name_)\
    public:\
        static\
        sb::StringSequence\
        get_type_names\
        (\
        )\
        {\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_base_of<Base, Self>::value\
                ),\
                "declared Self type not related to its base"\
            );\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_same<sb::AbstractObject, Self>::value ||\
                    std::is_base_of<sb::AbstractObject, Self>::value\
                ),\
                "declared name on a type not derived from sb::AbstractObject"\
            );\
            sb::StringSequence type_names = (\
                Self::get_type_names == get_type_names\
            ) ? (\
                Base::get_type_names()\
            ) : (\
                Self::get_type_names()\
            );\
            type_names.insert(type_names.begin(), name_);\
            return type_names;\
        }

#define SB_PROPERTIES(...)\
    public:\
        static\
        sb::ObjectPropertySequence\
        get_properties\
        (\
        )\
        {\
            SB_STATIC_ASSERT_MSG(\
                SB_EVAL(\
                    std::is_same<sb::AbstractObject, Self>::value ||\
                    std::is_base_of<sb::AbstractObject, Self>::value\
                ),\
                "declared properties on a type not derived from sb::AbstractObject"\
            );\
            sb::ObjectPropertySequence properties = {__VA_ARGS__};\
            sb::ObjectPropertySequence base_properties = (\
                Self::get_properties == get_properties\
            ) ? (\
                Base::get_properties()\
            ) : (\
                Self::get_properties()\
            );\
            properties.insert(\
                properties.begin(),\
                base_properties.begin(),\
                base_properties.end()\
            );\
            return properties;\
        }

#endif // SB_ABSTRACTOBJECT_H
