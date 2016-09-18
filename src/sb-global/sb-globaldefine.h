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
/*! \file sb-globaldefine.h */
#ifndef SB_GLOBALDEFINE_H
#define SB_GLOBALDEFINE_H

/// \cond INTERNAL

// os detection, see:
// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system

#define SB_OS_IS_MAC 0
#define SB_OS_IS_WIN 0
#define SB_OS_IS_LINUX 0

#if defined(__APPLE__) && defined(__MACH__)
#   undef SB_OS_IS_MAC
#   define SB_OS_IS_MAC 1
#elif defined(_WIN32)
#   undef SB_OS_IS_WIN
#   define SB_OS_IS_WIN 1
#elif defined(__linux__)
#   undef SB_OS_IS_LINUX
#   define SB_OS_IS_LINUX 1
#else
#   error "Softbloks doesn't support this OS for now: ask for it"
#endif

// compiler detection

# define SB_COMPILER_IS_GNU 0
# define SB_COMPILER_IS_MSVC 0

#if defined(__GNUC__)
#   undef SB_COMPILER_IS_GNU
#   define SB_COMPILER_IS_GNU 1

#elif defined(_MSC_VER)
#   undef SB_COMPILER_IS_MSVC
#   define SB_COMPILER_IS_MSVC 1
#else
#   error "Softbloks doesn't support this compiler for now: ask for it"
#endif

// visibility support

#if SB_COMPILER_IS_MSVC
#   define SB_DECL_EXPORT       __declspec(dllexport)
#   define SB_DECL_IMPORT       __declspec(dllimport)
#elif SB_COMPILER_IS_GNU
#   if SB_OS_IS_WIN
#       define SB_DECL_EXPORT   __declspec(dllexport)
#       define SB_DECL_IMPORT   __declspec(dllimport)
#   else
#       define SB_DECL_EXPORT   __attribute__((visibility("default")))
#       define SB_DECL_IMPORT   __attribute__((visibility("default")))
#       define SB_DECL_HIDDEN   __attribute__((visibility("hidden")))
#   endif
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
#define SB_OS_IS_MAC
/// Defined on all supported versions of Windows.
#define SB_OS_IS_WIN
/// Defined on all supported versions of Linux.
#define SB_OS_IS_LINUX

# define SB_COMPILER_IS_GNU
# define SB_COMPILER_IS_MSVC

/// This macro declares a symbol to be an export from a shared library.
#define SB_DECL_EXPORT
/// This macro declares a symbol to be an import from a shared library.
#define SB_DECL_IMPORT
/// This macro declares a symbol to be hidden in a shared library.
#define SB_DECL_HIDDEN

#endif

#endif // SB_GLOBALDEFINE_H
