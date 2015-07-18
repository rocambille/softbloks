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
#ifndef SB_EXECUTIVE_H
#define SB_EXECUTIVE_H

#include <sb-core/sb-abstractexecutive.h>

namespace sb
{

class SB_CORE_API PushExecutive : public AbstractExecutive
{

    SB_DECLARE_OBJECT(PushExecutive, "sb::PushExecutive")

public:

    class Private;

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

    static
    void
    construct
    (
        PushExecutive* this_
    );

    Private*
    d_ptr;

};

class SB_CORE_API PullExecutive : public AbstractExecutive
{

    SB_DECLARE_OBJECT(PullExecutive, "sb::PullExecutive")

public:

    class Private;

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

    static
    void
    construct
    (
        PullExecutive* this_
    );

    Private*
    d_ptr;

};

class SB_CORE_API PushPullExecutive : public AbstractExecutive
{

    SB_DECLARE_OBJECT(PushPullExecutive, "sb::PushPullExecutive")

public:

    class Private;

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

    static
    void
    construct
    (
        PushPullExecutive* this_
    );

    Private*
    d_ptr;

};

}

#endif // SB_EXECUTIVE_H
