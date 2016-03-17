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
/*! \file sb-global.h */
#ifndef SB_GLOBAL_H
#define SB_GLOBAL_H

// version numbers

#include <sb-global/sb-version.h>

namespace sb
{

/// Turns \a major_, \a minor_ and \a patch_ into a version number of the form
/// 0xMMNNPP where MM, NN and PP represent respectively the major, the minor
/// and the patch. This can be compared with another similarly formed version
/// number.
///
/// \sa VERSION and BUILD.
inline
int
version_check
(
    int major_,
    int minor_,
    int patch_ = 0
)
{
    return (
        (major_ << 16)|
        (minor_ <<  8)|
        (patch_)
    );
}

/// Constant symbolizing Softbloks's version number.
///
/// The value is of the form 0xMMNN00 where MM and NN represent respectively
/// the major and the minor.
///
/// \sa version_check(), VERSION_MAJOR, VERSION_MINOR and BUILD.
const int VERSION = sb::version_check(
    sb::VERSION_MAJOR,
    sb::VERSION_MINOR
);

/// Constant symbolizing Softbloks's build number.
///
/// The value is of the form 0xYYMMDD where YY, MM and DD represent
/// respectively the year, the month and the day.
///
/// \sa version_check(), BUILD_YEAR, BUILD_MONTH, BUILD_DAY and VERSION.
const int BUILD = sb::version_check(
    sb::BUILD_YEAR,
    sb::BUILD_MONTH,
    sb::BUILD_DAY
);

}

/// \cond INTERNAL

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

// visibility support

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

/// \endcond

#ifdef DOXYGEN

/// Defined on all supported versions of systems distributed by Apple.
#define SB_OS_MAC
/// Defined on all supported versions of Windows.
#define SB_OS_WIN
/// Defined on all supported versions of Linux.
#define SB_OS_LINUX

/// This macro declares a symbol to be an export from a shared library.
#define SB_DECL_EXPORT
/// This macro declares a symbol to be an import from a shared library.
#define SB_DECL_IMPORT
/// This macro declares a symbol to be hidden in a shared library.
#define SB_DECL_HIDDEN

#endif

#endif // SB_GLOBAL_H
