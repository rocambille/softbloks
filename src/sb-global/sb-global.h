/*
Copyright (C) 2014 Bastien Oudot

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
#ifndef SB_GLOBAL_H
#define SB_GLOBAL_H

// version macros

#include <sb-global/sb-version.h>

#define SB_VERSION_(major_, minor_)\
    ((major_<<8)|(minor_))

#define SB_VERSION SB_VERSION_(SB_VERSION_MAJOR, SB_VERSION_MINOR)

#define SB_BUILD_(year_, month_, day_)\
    ((year_<<16)|(month_<<8)|(day_))

#define SB_BUILD SB_BUILD_(SB_BUILD_YEAR, SB_BUILD_MONTH, SB_BUILD_DAY)

// os detection, see:
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system

#if defined(__APPLE__) && defined(__MACH__)
#   define SB_OS_MAC
#elif defined(_WIN32)
#   define SB_OS_WIN
#elif defined(__linux__)
#   define SB_OS_LINUX
#else
#   error "Softbloks doesn't support this OS for now: ask for it"
#endif

// attribute definition

#if defined(_MSC_VER)
#   define SB_DECL_EXPORT       __declspec(dllexport)
#   define SB_DECL_IMPORT       __declspec(dllimport)
#elif defined(__GNUC__)
#   ifdef SB_OS_WIN
#       define SB_DECL_EXPORT   __declspec(dllexport)
#       define SB_DECL_IMPORT   __declspec(dllimport)
#   else
#       define SB_DECL_EXPORT   __attribute__((visibility("default")))
#       define SB_DECL_IMPORT   __attribute__((visibility("default")))
#       define SB_DECL_HIDDEN   __attribute__((visibility("hidden")))
#   endif
#else
#   error "Softbloks doesn't support this compiler for now: ask for it"
#endif

#ifndef SB_DECL_EXPORT
#   define SB_DECL_EXPORT
#endif
#ifndef SB_DECL_IMPORT
#   define SB_DECL_IMPORT
#endif
#ifndef SB_DECL_HIDDEN
#   define SB_DECL_HIDDEN
#endif

#endif // SB_GLOBAL_H
