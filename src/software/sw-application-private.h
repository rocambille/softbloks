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
#ifndef SW_APPLICATION_PRIVATE_H
#define SW_APPLICATION_PRIVATE_H

#include "sw-application.h"

namespace sw
{

class ApplicationPrivate : public QObject
{

    Q_OBJECT

    Q_DECLARE_PUBLIC(Application)

public:

    ApplicationPrivate
    (
        Application* q_ptr_
    );

public:

    Application*
    q_ptr;

};

}

#endif // SW_APPLICATION_PRIVATE_H
