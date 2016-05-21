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

class Any
{

public:

    Any
    (
    ):
        content(SB_NULLPTR)
    {
    }

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

    Any
    (
        Any&& other_
    ):
        content(other_.content)
    {
        other_.content = SB_NULLPTR;
    }

    template<typename T>
    Any
    (
        const T& value_
    ):
        content(new Holder<T>(value_))
    {
    }

    template<typename T>
    Any
    (
        T&& value_
    ):
        content(new Holder<T>(std::forward<T>(value_)))
    {
    }

    ~Any
    (
    )
    {
        delete this->content;
    }

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

    template<typename T>
    Any&
    operator=
    (
        const T& value_
    )
    {
        delete this->content;

        this->content = new Holder<T>(value_);

        return (*this);
    }

    template<typename T>
    Any&
    operator=
    (
        T&& value_
    )
    {
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

    template<typename T>
    struct Holder : Placeholder
    {

        Holder
        (
            const T& value_
        ):
            held(value_)
        {
        }

        Holder
        (
            T&& value_
        ):
            held(std::forward<T>(value_))
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

    template<typename T>
    friend
    inline
    const T*
    any_cast
    (
        const Any* value_
    );

    template<typename T>
    friend
    inline
    T*
    any_cast
    (
        Any* value_
    );

    Placeholder*
    content;

};

template<typename T>
inline
T
any_cast
(
    const Any& value_
)
{
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

    if(
        value_ &&
        value_->content &&
        value_->content->get_type_info() == typeid(T)
    )
    {
        // note: when dynamic_cast is used on references,
        // it throws the exception std::bad_cast on failure
        result = &(
            static_cast<const Any::Holder<T>&>(
                *(value_->content)
            ).held
        );
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

    if(
        value_ &&
        value_->content &&
        value_->content->get_type_info() == typeid(T)
    )
    {
        // note: when dynamic_cast is used on references,
        // it throws the exception std::bad_cast on failure
        result = &(
            static_cast<Any::Holder<T>&>(
                *(value_->content)
            ).held
        );
    }

    return result;
}

}

#endif // SB_ANY_H
