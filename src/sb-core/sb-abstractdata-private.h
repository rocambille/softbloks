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
#ifndef SB_ABSTRACTDATA_PRIVATE_H
#define SB_ABSTRACTDATA_PRIVATE_H

#include <sb-core/sb-abstractdata.h>

#include <sb-core/sb-abstractblok.h>

#include <unordered_map>

namespace sb
{

using FollowerCollection = std::unordered_multimap<AbstractBlok*, Index>;

namespace Unmapper
{

    inline
    AbstractBlok*
    blok
    (
        const FollowerCollection::value_type& value_
    )
    {
        return value_.first;
    }

    inline
    Index
    input_index
    (
        const FollowerCollection::value_type& value_
    )
    {
        return value_.second;
    }

}

class SB_DECL_HIDDEN AbstractData::Private
{

public:

    Private
    (
        AbstractData* q_ptr_
    );

    template<typename T>
    static
    auto
    from
    (
        T&& this_
    )
    {
        return this_->AbstractData::d_ptr;
    }

public:

    AbstractData*
    q_ptr;

    AbstractBlok*
    source_blok;

    Index
    source_index;

    FollowerCollection
    followers;

};

}

#endif // SB_ABSTRACTDATA_PRIVATE_H
