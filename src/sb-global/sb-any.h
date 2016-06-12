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
#ifndef SB_ANY_H
#define SB_ANY_H

#include <sb-global/sb-globaldefine.h>

#include <type_traits>
#include <typeinfo>
#include <utility>

namespace sb
{

// one day: using Any = std::any;

/// \brief The class Any is a type-safe, generic container for single values.
///
/// Kevlin Henney (["Valued Conversions"](http://www.two-sdg.demon.co.uk/curbralan/papers/ValuedConversions.pdf)
/// from Mechanism to Method column in C++ Report 12(7), July–August 2000)
class Any
{

public:

    /// Constructs en empty object with null content and void type.
    ///
    /// \sa operator=, get_type_info() and any_cast().
    Any
    (
    ):
        content(SB_NULLPTR)
    {
    }

    /// Copies the content of \a other_ into this object.
    ///
    /// After construction, the type and the value held by this object are
    /// equivalent to the type and the value held by \a other_. Notably this
    /// object is empty if \a other_ is empty.
    ///
    /// \sa operator=, get_type_info() and any_cast().
    Any
    (
        const Any& other_
    ):
        content(SB_NULLPTR)
    {
        if(other_.content)
        {
            this->content = other_.content->clone();
        }
    }

    /// Moves the content of \a other_ into this object.
    ///
    /// After construction, \a other_ is empty while the type and the value
    /// held by this object are equivalent to the type and the value held by
    /// \a other_ before construction. Notably this object is empty if
    /// \a other_ was empty.
    ///
    /// \sa operator=, get_type_info() and any_cast().
    Any
    (
        Any&& other_
    ):
        content(other_.content)
    {
        other_.content = SB_NULLPTR;
    }

    /// Constructs an object with content direct-initialized from
    /// \a std::forward<T>(value_).
    ///
    /// Note that \a std::decay_t<ValueType> must be copy-constructible.
    ///
    /// \sa operator=, get_type_info() and any_cast().
    template<typename T>
    Any
    (
        T&& value_
    ):
        content(new Holder<T>(std::forward<T>(value_)))
    {
        SB_STATIC_ASSERT(
            std::is_copy_constructible<std::decay<T>::type>::value
        );
    }

    /// Destroys this object and its content, if any.
    ///
    /// \sa clear().
    ~Any
    (
    )
    {
        delete this->content;
    }

    /// Assigns \a other_ to this object by copying its content and returns a
    /// reference to this object.
    ///
    /// After assignment, the type and the value held by this object are
    /// equivalent to the type and the value held by \a other_. Notably this
    /// object is empty if \a other_ is empty.
    ///
    /// \sa Any(), get_type_info() and any_cast().
    Any&
    operator=
    (
        const Any& other_
    )
    {
        delete this->content;

        if(other_.content)
        {
            this->content = other_.content->clone();
        }
        else
        {
            this->content = SB_NULLPTR;
        }

        return (*this);
    }

    /// Assigns \a other_ to this object by moving its content and returns a
    /// reference to this object.
    ///
    /// After assignment, \a other_ is empty while the type and the value held
    /// by this object are equivalent to the type and the value held by
    /// \a other_ before assignment. Notably this object is empty if \a other_
    /// was empty.
    ///
    /// \sa Any(), get_type_info() and any_cast().
    Any&
    operator=
    (
        Any&& other_
    )
    {
        delete this->content;

        this->content = other_.content;
        other_.content = SB_NULLPTR;

        return (*this);
    }

    /// Assigns the type and the value of \a std::forward<T>(value_) to this
    /// object.
    ///
    /// Note that \a std::decay_t<ValueType> must be copy-constructible.
    ///
    /// \sa Any(), get_type_info() and any_cast().
    template<typename T>
    Any&
    operator=
    (
        T&& value_
    )
    {
        SB_STATIC_ASSERT(
            std::is_copy_constructible<std::decay<T>::type>::value
        );

        delete this->content;

        this->content = new Holder<T>(std::forward<T>(value_));

        return (*this);
    }

    // modifiers

    void
    clear
    (
    )
    {
        delete this->content;
        this->content = SB_NULLPTR;
    }

    void
    swap
    (
        Any& other_
    )
    {
        std::swap(
            this->content,
            other_.content
        );
    }

    // observers

    bool
    is_empty
    (
    )
    const
    {
        return this->content == SB_NULLPTR;
    }

    const std::type_info&
    get_type_info
    (
    )
    const
    {
        return (
            this->content ? this->content->get_type_info() : typeid(void)
        );
    }

private:

    /// \cond INTERNAL
    struct Placeholder
    {

        virtual
        ~Placeholder
        (
        )
        {
        }

        virtual
        const std::type_info&
        get_type_info
        (
        )
        const = 0;

        virtual
        Placeholder*
        clone
        (
        )
        const = 0;

    };
    /// \endcond

    /// \cond INTERNAL
    template<typename T>
    struct Holder : Placeholder
    {

        template<typename U>
        Holder
        (
            U&& value_
        ):
            held(std::forward<U>(value_))
        {
        }

        virtual
        const std::type_info&
        get_type_info
        (
        )
        const
        SB_OVERRIDE
        {
            return typeid(T);
        }

        virtual
        Placeholder*
        clone
        (
        )
        const
        SB_OVERRIDE
        {
            return new Holder(this->held);
        }

        T
        held;

    };
    /// \endcond

    /// \cond INTERNAL
    template<typename T>
    friend
    inline
    const T*
    any_cast
    (
        const Any* value_
    );
    /// \endcond

    /// \cond INTERNAL
    template<typename T>
    friend
    inline
    T*
    any_cast
    (
        Any* value_
    );
    /// \endcond

    /// \cond INTERNAL
    Placeholder*
    content;
    /// \endcond

};

class BadAnyCast : public std::bad_cast
{

public:

    virtual
    const char*
    what
    (
    )
    const
    SB_OVERRIDE
    {
        return "Bad any cast";
    }

};

template<typename T>
inline
T
any_cast
(
    const Any& value_
)
{
    SB_STATIC_ASSERT_MSG(
        SB_EVAL(
            !std::is_same<void, std::decay<T>::type>::value
        ),
        "invalid any_cast with type void"
    );
    return *(
        any_cast<
            typename std::add_const<
                typename std::remove_reference<
                    T
                >::type
            >::type
        >(&value_)
    );
}

template<typename T>
inline
T
any_cast
(
    Any& value_
)
{
    SB_STATIC_ASSERT_MSG(
        SB_EVAL(
            !std::is_same<void, std::decay<T>::type>::value
        ),
        "invalid any_cast with type void"
    );
    return *(
        any_cast<
            typename std::remove_reference<
                T
            >::type
        >(&value_)
    );
}

template<typename T>
inline
T
any_cast
(
    Any&& value_
)
{
    SB_STATIC_ASSERT_MSG(
        SB_EVAL(
            !std::is_same<void, std::decay<T>::type>::value
        ),
        "invalid any_cast with type void"
    );
    return *(
        any_cast<
            typename std::remove_reference<
                T
            >::type
        >(&value_)
    );
}

template<typename T>
inline
const T*
any_cast
(
    const Any* value_
)
{
    const T* result = SB_NULLPTR;

    if(value_)
    {
        if(value_->get_type_info() == typeid(T))
        {
            if(value_->content)
            {
                result = &(
                    static_cast<const Any::Holder<T>*>(
                        value_->content
                    )->held
                );
            }
        }
        else
        {
            throw BadAnyCast();
        }
    }

    return result;
}

template<typename T>
inline
T*
any_cast
(
    Any* value_
)
{
    T* result = SB_NULLPTR;

    if(value_)
    {
        if(value_->get_type_info() == typeid(T))
        {
            if(value_->content)
            {
                result = &(
                    static_cast<Any::Holder<T>*>(
                        value_->content
                    )->held
                );
            }
        }
        else
        {
            throw BadAnyCast();
        }
    }

    return result;
}

}

#endif // SB_ANY_H
