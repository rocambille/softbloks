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
#ifndef SR_LIBRARY_PRIVATE_H
#define SR_LIBRARY_PRIVATE_H

#include "sr-library.h"

#include <map>

namespace sr
{

#define SR_DECLARE_GLOBAL_DESTRUCTOR(to_call_)\
    namespace\
    {\
\
    static const struct to_call_##_Destructor\
    {\
        inline\
        to_call_##_Destructor\
        (\
        )\
        {\
        }\
        inline\
        ~ to_call_##_Destructor\
        (\
        )\
        {\
            to_call_();\
        }\
    } to_call_##instance;\
\
    }

class Library::Private
{

public:

    Private
    (
        const std::string& file_path_
    );

    bool
    load
    (
    );

    bool
    unload
    (
    );

    FunctionPointer
    resolve
    (
        const char* symbol_
    );

    static
    void
    attach
    (
        Library* object_,
        const std::string& file_path_
    );

    static
    void
    detach
    (
        Library* object_
    );

public:

    int
    reference_count;

    std::string
    file_path;

    void*
    handle;

    static
    std::map<std::string, Private*>
    instances;

};

std::map<std::string, Library::Private*> Library::Private::instances;

}

#endif // SR_LIBRARY_PRIVATE_H
