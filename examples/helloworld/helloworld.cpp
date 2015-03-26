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
#include "sb-core.h"

class Soft1 : public sb::AbstractSoft
{

    SB_DECLARE_OBJECT(Soft1, "Soft1")

    static
    void
    construct
    (
        Soft1* this_
    )
    {
    }

};

class Soft2 : public sb::AbstractSoft
{

    SB_DECLARE_OBJECT(Soft2, "Soft2")

    static
    void
    construct
    (
        Soft2* this_
    )
    {
    }

};

SB_DECLARE_MODULE(helloworld)
{
    sb::register_object<Soft1>();
    sb::register_object<Soft2>();
}
