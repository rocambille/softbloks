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
#ifndef SR_DIRECTORY_PRIVATE_H
#define SR_DIRECTORY_PRIVATE_H

#include "sr-directory.h"

#include <sb-global/sb-global.h>

namespace sr
{

class Directory::Private
{

public:

    Private
    (
        Directory* q_ptr_
    );

public:

    Directory*
    q_ptr;

    std::string
    path;

#ifdef SB_OS_WIN
    std::string
    windows_path;
#endif

};

}

#endif // SR_DIRECTORY_PRIVATE_H
