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
#ifndef SR_LIBRARY_H
#define SR_LIBRARY_H

#include <string>

namespace sr
{

using FunctionPointer = void(*)();

class Library
{

public:

    class Private;

    Library
    (
        const std::string& file_path_ = std::string()
    );
    Library
    (
        const Library& other_
    );

    ~Library
    (
    );

    Library&
    operator=
    (
        const Library& other_
    );

    const std::string&
    get_file_path
    (
    )
    const;

    void
    set_file_path
    (
        const std::string& file_path_
    );

    bool
    is_loaded
    (
    )
    const;

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
    FunctionPointer
    resolve
    (
        const std::string& file_path_,
        const char* symbol_
    );

private:

    Private*
    d_ptr;

};

}

#endif // SR_LIBRARY_H
