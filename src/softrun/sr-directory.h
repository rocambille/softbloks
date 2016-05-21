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
#ifndef SR_DIRECTORY_H
#define SR_DIRECTORY_H

#include <string>
#include <vector>

namespace sr
{

class Directory
{

public:

    class Private;

    using EntrySequence = std::vector<std::string>;

    Directory
    (
        const std::string& path_ = std::string()
    );
    Directory
    (
        const Directory& other_
    );

    ~Directory
    (
    );

    Directory&
    operator=
    (
        const Directory& other_
    );

    std::string
    get_path
    (
    )
    const;

    void
    set_path
    (
        const std::string& path_
    );

    bool
    exists
    (
    )
    const;

    EntrySequence
    get_entry_sequence
    (
    )
    const;

private:

    Private*
    d_ptr;

};

}

#endif // SR_DIRECTORY_H
