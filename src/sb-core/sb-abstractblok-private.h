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
#ifndef SB_ABSTRACTBLOK_PRIVATE_H
#define SB_ABSTRACTBLOK_PRIVATE_H

#include <sb-core/sb-abstractblok.h>

#include <sb-core/sb-abstractdata.h>

namespace sb
{

class SB_DECL_HIDDEN AbstractBlok::Private
{

public:

    Private
    (
        AbstractBlok* q_ptr_
    );

    void
    set_inputs_formats
    (
        const ObjectFormatSequence& value_
    );

    void
    set_outputs_type_names
    (
        const StringSequence& value_
    );

    SharedData
    lock_input
    (
        Index index_
    )
    const;

    bool
    set_input
    (
        Index index_,
        const SharedData& value_
    );

    void
    unlink_input
    (
        Index index_
    );

    static
    Private*
    from
    (
        const AbstractBlok* this_
    );

    static
    Private*
    from
    (
        const UniqueBlok& this_
    );

public:

    AbstractBlok*
    q_ptr;

    ObjectFormatSequence
    inputs_formats;

    std::vector<WeakData>
    inputs;

    ObjectFormatSequence
    outputs_formats;

    std::vector<SharedData>
    outputs;

    UniqueExecutive
    executive;

};

}

#endif // SB_ABSTRACTBLOK_PRIVATE_H
