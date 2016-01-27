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

#include <array>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <typeindex>
#include <vector>

#ifdef sb_core_EXPORTS
#define SB_CORE_API SB_DECL_EXPORT
#else
#define SB_CORE_API SB_DECL_IMPORT
#endif

namespace sb
{

enum Mode
{
    READ_ONLY   = 0x1,
    WRITE_ONLY  = 0x2,
    READ_WRITE  = 0x3,
};

using IndexRange = std::array<double, 2>;

using IndexRangeConverter =
    std::function<sb::IndexRange(const std::vector<sb::IndexRange>&)>;

using IndexCollection = std::vector<double>;

using IndexCollectionConverter =
    std::function<sb::IndexCollection(const std::vector<sb::IndexCollection>&)>;

const size_t infinity = std::numeric_limits<size_t>::max();

template<typename U, typename T, typename D>
std::unique_ptr<U, D>
static_pointer_cast
(
    std::unique_ptr<T, D>&& t_ptr_
)
{
    auto ptr = static_cast<U*>(t_ptr_.get());

    std::unique_ptr<U, D> u_ptr(
        ptr, std::move(t_ptr_.get_deleter())
    );

    t_ptr_.release();

    return u_ptr;
}

template <typename T>
std::vector<T>
join
(
    const std::vector<T>& a_,
    const std::vector<T>& b_
)
{
    std::vector<T> ab;
    ab.reserve(a_.size() + b_.size());

    ab.insert(ab.end(), a_.begin(), a_.end());
    ab.insert(ab.end(), b_.begin(), b_.end());

    return ab;
}

struct PropertyFormat
{

    std::type_index
    type;

    sb::Mode
    mode;

};

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
        left_.mode == right_.mode
    );
}

using PropertyFormatMap = std::map<std::string, sb::PropertyFormat>;

struct ObjectFormat
{

    std::vector<std::string>
    type_names;

    sb::PropertyFormatMap
    properties;

};

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

inline
bool
operator>>
(
    const sb::ObjectFormat& from_,
    const sb::ObjectFormat& to_extract_
)
{
    return (
        to_extract_.type_names.size() > 0
    ) && std::all_of(
        to_extract_.type_names.begin(),
        to_extract_.type_names.end(),
        [&from_]
        (
            const std::string& type_name_
        )
        {
            return std::find(
                from_.type_names.begin(),
                from_.type_names.end(),
                type_name_
            ) != from_.type_names.end();
        }
    ) && std::all_of(
        to_extract_.properties.begin(),
        to_extract_.properties.end(),
        [&from_]
        (
            const sb::PropertyFormatMap::value_type& value_to_extract_
        )
        {
            return std::find_if(
                from_.properties.begin(),
                from_.properties.end(),
                [&value_to_extract_]
                (
                    const sb::PropertyFormatMap::value_type& from_value_
                )
                {
                    return (
                        value_to_extract_.first
                    ) == (
                        from_value_.first
                    ) && (
                        value_to_extract_.second.type
                    ) == (
                        from_value_.second.type
                    ) && (
                        value_to_extract_.second.mode &
                        from_value_.second.mode
                    ) == (
                        value_to_extract_.second.mode
                    );
                }
            ) != from_.properties.end();
        }
    );
}

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

const sb::ObjectFormat
undefined_object_format = { { } };

const sb::ObjectFormat
any_object_format = { { "sb.AbstractObject" } };

template<typename T>
class Meta
{

public:

    static
    std::vector<std::string>
    object_names;

};

template<typename T>
inline
std::string
get_object_name
(
)
{
    return Meta<T>::object_names[0];
}

}

#define SB_ROOT(attributes_, type_, name_)\
    class type_;\
    std::vector<std::string> sb::Meta<type_>::object_names = {name_};\
    class attributes_ type_ : public sb::Meta<type_>

#define SB_CLASS(attributes_, type_, name_, super_class_)\
    class type_;\
    std::vector<std::string> sb::Meta<type_>::object_names = sb::join(\
        std::vector<std::string>({name_}),\
        sb::Meta<super_class_>::object_names\
    );\
    class attributes_ type_ : public sb::Meta<type_>, public super_class_

#define SB_DECLARE_MODULE(descriptor_)\
    namespace Global\
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
        return Global::descriptor;\
    }\
    extern "C"\
    SB_DECL_EXPORT\
    void\
    sb_main\
    (\
    )

#endif // SB_COREDEFINE_H
