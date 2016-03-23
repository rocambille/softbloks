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

#include <sb-core/sb-coredefine.h>

#include <algorithm>
#include <string>

namespace sb
{

/// \brief The AbstractObject class is the base class for all Softbloks
/// objects.
///
/// It provides common features including dynamic properties.
///
/// \anchor property-system
/// ### The Property System ###
///
/// Dynamic properties can be associated to a Softbloks object at runtime.
/// Properties can be added and removed to any instance of a class based on
/// AbstractObject:
///
/// \code{cpp}
/// class Foo : public sb::AbstractObject
/// {
///     Foo()
///     {
///         // register a property on this instance
///
///         this->register_property<int>(
///             "my_property",
///             sb::READ_WRITE,
///             [](){ return 0; },  // reading accessor
///             [](const int&){ }   // writing accessor
///         );
///
///         // the property can now be accessed using get() and set()
///
///         this->get<int>("my_property");
///         this->set<int>("my_property", 42);
///
///         // we don't need it anymore: unregister it
///
///         this->unregister_property("my_property");
///     }
/// };
/// \endcode
///
/// Properties can also be registered on an instance by an outside object. An
/// ID must then be provided to prevent the property to be unregistered by
/// someone else:
///
/// \code{cpp}
/// class Bar
/// {
///     Bar()
///     {
///         // create an instance of Foo
///
///         auto foo = sb::create_unique_object(
///             sb::get_type_name<Foo>()
///         );
///
///         // register a property on it
///
///         foo->register_property<int>(
///             this, // we must provide here an ID to protect the property
///             "my_property",
///             sb::READ_WRITE,
///             [](){ return 0; },  // reading accessor
///             [](const int&){ }   // writing accessor
///         );
///
///         ...
///
///         // unregister must be called with the registration ID:
///         // the property can't be unregistered by unfamiliars
///
///         foo->unregister_property(this, "my_property");
///     }
/// };
/// \endcode
///
/// Through
/// [std::function](http://www.cplusplus.com/reference/functional/function/),
/// an accessor may be a function, a function pointer, a function object, a
/// lambda expression or a pointer to member -- bound to an instance using
/// [std::bind()](http://www.cplusplus.com/reference/functional/bind/).
///
/// \sa register_property() and SB_DECLARE_PROPERTIES().
class SB_CORE_API AbstractObject
{

public:

    /// Template alias for a reading accessor.
    ///
    /// \sa Set, register_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    using Get = std::function<T(void)>;

    /// Template alias for a writing accessor.
    ///
    /// \sa Get, register_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    using Set = std::function<void(const T&)>;

private:

    /// \cond INTERNAL
    template<typename T>
    struct Accessors
    {

        Get<T>
        get;

        Set<T>
        set;

    };
    /// \endcond

    /// \cond INTERNAL
    struct Property
    {

        void*
        owner;

        PropertyFormat
        format;

        std::shared_ptr<void>
        accessors;

    };
    /// \endcond

public:

    class Private;

    /// \cond INTERNAL
    // deletion of copy-constructor. copy-constructor for a derived class
    // won't work unless it is explicitly defined and it doesn't call this
    // constructor: by default, copy is disabled
    AbstractObject
    (
        const AbstractObject& other_
    )
    = delete;
    /// \endcond

    /// Constructs a Softbloks object.
    AbstractObject
    (
    );

    /// Destroys this object.
    virtual
    ~AbstractObject
    (
    );

    /// \cond INTERNAL
    // deletion of operator=. operator= for a derived class won't work unless
    // it is explicitly defined and it doesn't call this operator: by default,
    // copy is disabled
    AbstractObject&
    operator=
    (
        const AbstractObject& other_
    )
    = delete;
    /// \endcond

    /// Returns the format of this object.
    ///
    /// The returned format may differ from the one returned by
    /// get_object_format(): get_object_format() can be used to get the format
    /// of a known type without creating an instance -- so without executing
    /// any unreliable code like the type's constructor -- but it is only
    /// aware of the properties explicitly declared with
    /// SB_DECLARE_PROPERTIES(); get_instance_format() fills the format
    /// structure at runtime by listing all the properties registered on the
    /// instance -- including undeclared properties and properties registered
    /// at runtime on this instance.
    ///
    /// \sa \ref property-system "Softbloks's property system".
    ObjectFormat
    get_instance_format
    (
    )
    const;

    /// Returns current value of the property \a name_.
    ///
    /// An exception is raised if this object has no registered properties
    /// called \a name_, if the property was registered with a type different
    /// from \a T or if the property was registered in write-only mode.
    ///
    /// \sa set(), register_property(), unregister_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    T
    get
    (
        const std::string& name_
    )
    const
    {
        Property wanted_property = this->properties->at(name_);

        // check type

        std::type_index wanted_result_type = typeid(T);

        if(wanted_result_type != wanted_property.format.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: property " +
                name_ +
                " registered as " +
                wanted_property.format.type.name() +
                " is accessed as " +
                wanted_result_type.name()
            );
        }

        // check access mode

        if((wanted_property.format.access_mode & READ_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::get: " +
                "calling on property " +
                name_ +
                " which is write-only"
            );
        }

        // call is valid

        return std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->get();
    }

    /// Sets the property \a name_ to \a value_.
    ///
    /// An exception is raised if this object has no registered properties
    /// called \a name_, if the property was registered with a type different
    /// from \a T or if the property was registered in read-only mode.
    ///
    /// \sa get(), register_property(), unregister_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    void
    set
    (
        const std::string& name_,
        const T& value_
    )
    {
        Property wanted_property = this->properties->at(name_);

        // check type

        std::type_index wanted_argument_type = typeid(T);

        if(wanted_argument_type != wanted_property.format.type)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: property " +
                name_ +
                " registered as " +
                wanted_property.format.type.name() +
                " is accessed as " +
                wanted_argument_type.name()
            );
        }

        // check access mode

        if((wanted_property.format.access_mode & WRITE_ONLY) == 0)
        {
            throw std::invalid_argument(
                std::string() +
                "sb::AbstractBlok::set: " +
                "calling on property " +
                name_ +
                " which is read-only"
            );
        }

        // call is valid

        std::static_pointer_cast< Accessors<T> >(
            wanted_property.accessors
        )->set(value_);
    }

    /// Registers a property of type \a T called \a name_ on this object.
    ///
    /// \a access_mode_ specifies how the property can be accessed: read-only,
    /// write-only or read and write.
    ///
    /// \a get_ points to a reading accessor -- pass \a nullptr for a
    /// write-only property.
    /// \a set_ points to a writing accessor -- pass \a nullptr for a
    /// read-only property.
    ///
    /// \a owner_ is used to identify the caller: you need to provide the same
    /// ID when unregistering a property.
    ///
    /// The function returns \b true on success, i.e. if there was no
    /// previously registered properties called \a name_ on this object; it
    /// returns \b false otherwise.
    ///
    /// \sa get(), set(), unregister_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    bool
    register_property
    (
        void* owner_,
        const std::string& name_,
        AccessMode access_mode_,
        const Get<T>& get_,
        const Set<T>& set_
    )
    {
        bool registered = false;

        if(this->properties->count(name_) == 0)
        {
            // initialize the new property

            auto accessors = std::make_shared< Accessors<T> >();
            accessors->get = get_;
            accessors->set = set_;

            Property new_property = {
                owner_,
                typeid(T),
                access_mode_,
                std::static_pointer_cast<void>(
                    accessors
                )
            };

            // store the property

            this->properties->emplace(name_, new_property);

            registered = true;
        }

        return registered;
    }

    /// Unregisters the property \a name_ on this object.
    ///
    /// The function returns \b true on success, i.e. if the property exists
    /// and has been registered on this object by \a owner_; it returns
    /// \b false otherwise.
    ///
    /// \sa register_property() and
    /// \ref property-system "Softbloks's property system".
    bool
    unregister_property
    (
        void* owner_,
        const std::string& name_
    );

protected:

    /// This function overloads register_property().
    ///
    /// Registers a property owned by \a this. This is equivalent to:
    /// ```
    /// this->register_property(this, name_, access_mode_, get_, set_);
    /// ```
    /// \sa get(), set(), unregister_property() and
    /// \ref property-system "Softbloks's property system".
    template<typename T>
    inline
    bool
    register_property
    (
        const std::string& name_,
        AccessMode access_mode_,
        const Get<T>& get_,
        const Set<T>& set_
    )
    {
        return this->register_property<T>(
            this,
            name_,
            access_mode_,
            get_,
            set_
        );
    }

    /// This function overloads unregister_property().
    ///
    /// Unregisters a property owned by \a this. This is equivalent to:
    /// ```
    /// this->unregister_property(this, name_);
    /// ```
    /// \sa register_property() and
    /// \ref property-system "Softbloks's property system".
    bool
    unregister_property
    (
        const std::string& name_
    );

    /// \cond INTERNAL
    template<typename T>
    friend
    bool
    register_object
    (
    );
    /// \endcond

private:

    /// \cond INTERNAL
    static
    void
    set_type_names
    (
        AbstractObject* this_,
        std::vector<std::string> names_
    );
    /// \endcond

    /// \cond INTERNAL
    static
    void
    init
    (
        AbstractObject* this_
    );
    /// \endcond

    /// \cond INTERNAL
    static
    void
    forget
    (
        AbstractObject* this_
    );
    /// \endcond

    /// \cond INTERNAL
    static
    bool
    register_object
    (
        const ObjectFormat& format_,
        const std::function<Unique<AbstractObject>(void)>& factory_
    );
    /// \endcond

    /// \cond INTERNAL
    using PropertyMap = std::map<std::string, Property>;

    PropertyMap*
    properties;

    Private*
    d_ptr;
    /// \endcond

};

/// Template alias for a managed object with shared ownership.
using SharedObject = Shared<AbstractObject>;

/// Returns a managed pointer to an instance of the object designated by
/// \a name_.
///
/// If \a name_ is not a valid name associated to an object with
/// SB_DECLARE_CLASS() or if the object wasn't registered using
/// register_object() before calling this function, the returned pointer will
/// be null.
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
/// If \a name_ is not a valid name associated to an object with
/// SB_DECLARE_CLASS() or if the object wasn't registered using
/// register_object() before calling this function, the returned pointer will
/// be null.
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

/// Template alias for a managed object uniquely owned.
using UniqueObject = Unique<AbstractObject>;

/// Returns a managed pointer to an instance of the object designated by
/// \a name_.
///
/// If \a name_ is not a valid name associated to an object with
/// SB_DECLARE_CLASS() or if the object wasn't registered using
/// register_object() before calling this function, the returned pointer will
/// be null.
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
/// If \a name_ is not a valid name associated to an object with
/// SB_DECLARE_CLASS() or if the object wasn't registered using
/// register_object() before calling this function, the returned pointer will
/// be null.
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
    return sb::static_move_cast<T>(
        create_unique_object(name_)
    );
}

/// Registers the type \a T as an instantiable object using
/// create_shared_object() or one of the associated functions.
///
/// The type \a T must be declared with SB_DECLARE_CLASS() before
/// it can be registered.
///
/// The function returns \b true if \a T was not previously registered; it
/// returns \b false otherwise.
///
/// \sa unregister_all_objects().
template<typename T>
inline
bool
register_object
(
)
{
    auto factory = (
        []
        (
        )
        {
            auto instance = UniqueObject(
                new T,
                []
                (
                    AbstractObject* ptr_
                )
                {
                    delete ptr_;

                    AbstractObject::forget(ptr_);
                }
            );

            AbstractObject::set_type_names(
                instance.get(),
                get_type_names<T>()
            );
            AbstractObject::init(instance.get());

            return instance;
        }
    );

    return AbstractObject::register_object(
        {
            get_type_names<T>(),
            get_properties<T>()
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
/// \sa SB_DECLARE_CLASS() and SB_DECLARE_PROPERTIES().
SB_CORE_API
std::vector<std::string>
get_registered_object_names
(
    const ObjectFormat& filter_ = ANY_OBJECT_FORMAT
);

/// Returns the format of the object identified by \a name_.
///
/// The object must have been previously registered using register_object().
/// The function returns UNDEFINED_OBJECT_FORMAT otherwise.
///
/// The returned format may differ from the one returned by
/// get_instance_format(): get_object_format() can be used to get the format
/// of a known type without creating an instance -- so without executing
/// any unreliable code like the type's constructor -- but it is only
/// aware of the properties explicitly declared with
/// SB_DECLARE_PROPERTIES(); get_instance_format() fills the format
/// structure at runtime by listing all the properties registered on the
/// instance -- including undeclared properties and properties registered
/// at runtime on this instance.
///
/// \sa \ref property-system "Softbloks's property system".
SB_CORE_API
ObjectFormat
get_object_format
(
    const std::string& name_
);

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

/// \cond INTERNAL
template<>
inline
std::vector<std::string>
get_type_names<AbstractObject>
(
)
{
    return {"sb.AbstractObject"};
}
/// \endcond

/// \cond INTERNAL
template<>
inline
sb::PropertyFormatMap
get_properties<AbstractObject>
(
)
{
    return {};
}
/// \endcond

}

#endif // SB_ABSTRACTOBJECT_H
