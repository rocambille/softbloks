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
#ifndef SB_EXECUTIVE_H
#define SB_EXECUTIVE_H

#include <sb-core/sb-abstractexecutive.h>

namespace sb
{

SB_CLASS(SB_CORE_API, PushExecutive, "sb.PushExecutive", AbstractExecutive)
{

public:

    class Private;

    PushExecutive
    (
    );

    virtual
    ~PushExecutive
    (
    );

    virtual
    void
    on_input_pushed
    (
        size_t index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        size_t index_
    )
    override;

private:

    Private*
    d_ptr;

};

SB_CLASS(SB_CORE_API, PullExecutive, "sb.PullExecutive", AbstractExecutive)
{

public:

    class Private;

    PullExecutive
    (
    );

    virtual
    ~PullExecutive
    (
    );

    virtual
    void
    on_input_pushed
    (
        size_t index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        size_t index_
    )
    override;

private:

    Private*
    d_ptr;

};

SB_CLASS(SB_CORE_API, PushPullExecutive, "sb.PushPullExecutive", AbstractExecutive)
{

public:

    class Private;

    PushPullExecutive
    (
    );

    virtual
    ~PushPullExecutive
    (
    );

    virtual
    void
    on_input_pushed
    (
        size_t index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        size_t index_
    )
    override;

private:

    Private*
    d_ptr;

};

}

#endif // SB_EXECUTIVE_H
