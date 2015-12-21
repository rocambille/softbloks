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
#include "sr-library.h"
#include "sr-library-private.h"

#include <sb-global/sb-global.h>

#include <map>

using namespace sr;

Library::Library
(
    const std::string& file_path_
)
{
    Private::attach(
        this,
        file_path_
    );
}

Library::Library
(
    const Library& other_
)
{
    Private::attach(
        this,
        other_.get_file_path()
    );
}

Library::~Library
(
)
{
    Private::detach(
        this
    );
}

Library&
Library::operator=
(
    const Library& other_
)
{
    Private::detach(
        this
    );

    Private::attach(
        this,
        other_.get_file_path()
    );

    return (*this);
}

const std::string&
Library::get_file_path
(
)
const
{
    return d_ptr->file_path;
}

void
Library::set_file_path
(
    const std::string& file_path_
)
{
    Private::detach(
        this
    );

    Private::attach(
        this,
        file_path_
    );
}

bool
Library::is_loaded
(
)
const
{
    return d_ptr->handle != nullptr;
}

bool
Library::load
(
)
{
    return d_ptr->load();
}

bool
Library::unload
(
)
{
    return d_ptr->unload();
}

FunctionPointer
Library::resolve
(
    const char* symbol_
)
{
    FunctionPointer resolved = nullptr;

    if(this->is_loaded() || this->load())
    {
        resolved = d_ptr->resolve(symbol_);
    }

    return resolved;
}

FunctionPointer
Library::resolve
(
    const std::string& file_path_,
    const char* symbol_
)
{
    return Library(file_path_).resolve(symbol_);
}

Library::Private::Private
(
    const std::string& file_path_
):
    file_path       (file_path_),
    handle          (nullptr),
    reference_count (0)
{
}

#ifdef SB_OS_WIN

#include <windows.h>
#include <algorithm>

#else // SB_OS_WIN

#include <dlfcn.h>

#endif // SB_OS_WIN

bool
Library::Private::load
(
)
{
    bool loaded = false;

    bool already_loaded = (this->handle != nullptr);

    if(!already_loaded && !this->file_path.empty())
    {
#ifdef SB_OS_WIN
        std::string windows_path = this->file_path;

        std::replace(
            windows_path.begin(),
            windows_path.end(),
            '/',
            '\\'
        );

        this->handle = static_cast<LPVOID>(
            LoadLibraryA(
                windows_path.c_str()
            )
        );
#else // SB_OS_WIN
        this->handle = dlopen(
            this->file_path.c_str(),
            RTLD_LAZY | RTLD_GLOBAL
        );
#endif // SB_OS_WIN

        loaded = (this->handle != nullptr);
    }

    if(loaded)
    {
        // increment reference count to disable handle destruction

        this->reference_count++;
    }

    return loaded;
}

bool
Library::Private::unload
(
)
{
    bool unloaded = false;

    if(this->handle != nullptr)
    {
        unloaded = (
#ifdef SB_OS_WIN
            FreeLibrary(
                static_cast<HMODULE>(this->handle)
            ) != FALSE
#else // SB_OS_WIN
            dlclose(
                this->handle
            ) == 0
#endif // SB_OS_WIN
        );

        this->handle = nullptr;
    }

    if(unloaded)
    {
        this->reference_count--;
    }

    return unloaded;
}

FunctionPointer
Library::Private::resolve
(
    const char* symbol_
)
{
    FunctionPointer resolved = nullptr;

    if(this->handle != nullptr)
    {
        resolved = reinterpret_cast<FunctionPointer>(
#ifdef SB_OS_WIN
            GetProcAddress(
                static_cast<HMODULE>(this->handle),
                symbol_
            )
#else // SB_OS_WIN
            dlsym(
                this->handle,
                symbol_
            )
#endif // SB_OS_WIN
        );
    }

    return resolved;
}

void
Library::Private::attach
(
    Library* object_,
    const std::string& file_path_
)
{
    if(Private::instances.count(file_path_) != 0)
    {
        object_->d_ptr = Private::instances[file_path_];
    }
    else
    {
        object_->d_ptr = new Library::Private(file_path_);

        Private::instances[file_path_] = object_->d_ptr;
    }

    object_->d_ptr->reference_count++;
}

void
Library::Private::detach
(
    Library* object_
)
{
    Private* d_ptr = object_->d_ptr;

    d_ptr->reference_count--;

    if(d_ptr->reference_count == 0)
    {
        Private::instances.erase(
            Private::instances.find(
                d_ptr->file_path
            )
        );

        delete d_ptr;
    }

    object_->d_ptr = nullptr;
}

static
void
unload_libraries
(
)
{
    for(auto instance : Library::Private::instances)
    {
        instance.second->unload();

        delete instance.second;
    }
}
SR_DECLARE_GLOBAL_DESTRUCTOR(unload_libraries)
