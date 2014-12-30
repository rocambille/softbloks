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
#ifndef SB_COREDEFINE_H
#define SB_COREDEFINE_H

#include "sb-global.h"

#ifdef sb_core_EXPORTS
#define SB_CORE_API SB_DECL_EXPORT
#else
#define SB_CORE_API SB_DECL_IMPORT
#endif

namespace sb
{

enum Mode
{
    READ_ONLY   = 0x1,
    WRITE_ONLY  = 0x2,
    READ_WRITE  = 0x3,
};

}

#endif // SB_COREDEFINE_H
