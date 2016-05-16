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
#include "sr-directory.h"
#include "sr-directory-private.h"

#include <algorithm>

using namespace sr;

Directory::Directory
(
    const std::string& path_
)
{
    d_ptr = new Private(this);

    this->set_path(path_);
}

Directory::Directory
(
    const Directory& other_
)
{
    d_ptr = new Private(this);

    this->set_path(d_ptr->path);
}

Directory::~Directory
(
)
{
    delete d_ptr;
}

Directory&
Directory::operator=
(
    const Directory& other_
)
{
    this->set_path(d_ptr->path);

    return (*this);
}

std::string
Directory::get_path
(
)
const
{
    return d_ptr->path;
}

void
Directory::set_path
(
    const std::string& path_
)
{
    d_ptr->path = path_;

    if(d_ptr->path.size() > 0)
    {
        if(
#ifdef SB_OS_WIN
            d_ptr->path[d_ptr->path.size()-1] != '\\' ||
#endif
            d_ptr->path[d_ptr->path.size()-1] != '/'
        )
        {
            d_ptr->path.append("/");
        }

#ifdef SB_OS_WIN
        std::replace(
            d_ptr->path.begin(),
            d_ptr->path.end(),
            '\\',
            '/'
        );

        d_ptr->windows_path = d_ptr->path;

        std::replace(
            d_ptr->windows_path.begin(),
            d_ptr->windows_path.end(),
            '/',
            '\\'
        );
#endif
    }
}


#ifdef SB_OS_WIN

#include <windows.h>

bool
Directory::exists
(
)
const
{
    bool result = false;

    auto file_attributes = GetFileAttributesA(
        d_ptr->windows_path.c_str()
    );

    if(
        file_attributes != INVALID_FILE_ATTRIBUTES &&
        (file_attributes & FILE_ATTRIBUTE_DIRECTORY) != 0
    )
    {
        result = true;
    }

    return result;
}

Directory::EntrySequence
Directory::get_entry_sequence
(
)
const
{
    EntrySequence entry_sequence;

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(
        d_ptr->windows_path.append("*").c_str(),
        &find_data
    );

    if(hFind != INVALID_HANDLE_VALUE)
    {
        entry_sequence.push_back(
            find_data.cFileName
        );

        while(FindNextFileA(hFind, &find_data) != FALSE)
        {
            entry_sequence.push_back(
                find_data.cFileName
            );
        }
    }

    FindClose(hFind);

    std::sort(
        entry_sequence.begin(),
        entry_sequence.end()
    );

    return entry_sequence;
}

#else // SB_OS_WIN

#include <sys/types.h>
#include <dirent.h>

bool
Directory::exists
(
)
const
{
    bool result = false;

    DIR* dir = opendir(d_ptr->path.c_str());

    if(dir != nullptr)
    {
        result = true;

        closedir(dir);
    }

    return result;
}

Directory::EntrySequence
Directory::get_entry_sequence
(
)
const
{
    EntrySequence entry_sequence;

    DIR* dir = opendir(d_ptr->path.c_str());

    if(dir != nullptr)
    {
        dirent *entry = readdir(dir);

        while(entry)
        {
            entry_sequence.push_back(entry->d_name);

            entry = readdir(dir);
        }

        closedir(dir);
    }

    std::sort(
        entry_sequence.begin(),
        entry_sequence.end()
    );

    return entry_sequence;
}

#endif // SB_OS_WIN

Directory::Private::Private
(
    Directory* q_ptr_
):
    q_ptr(q_ptr_)
{
}
