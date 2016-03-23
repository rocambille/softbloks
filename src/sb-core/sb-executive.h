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

class SB_CORE_API PushExecutive : public sb::AbstractExecutive
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
        sb::Index index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        sb::Index index_
    )
    override;

private:

    Private*
    d_ptr;

};

class SB_CORE_API PullExecutive : public sb::AbstractExecutive
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
        sb::Index index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        sb::Index index_
    )
    override;

private:

    Private*
    d_ptr;

};

class SB_CORE_API PushPullExecutive : public sb::AbstractExecutive
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
        sb::Index index_
    )
    override;

    virtual
    void
    on_output_pulled
    (
        sb::Index index_
    )
    override;

private:

    Private*
    d_ptr;

};

}

SB_DECLARE_CLASS(
    sb::PushExecutive,
    "sb.PushExecutive",
    sb::AbstractExecutive
)
SB_DECLARE_CLASS(
    sb::PullExecutive,
    "sb.PullExecutive",
    sb::AbstractExecutive
)
SB_DECLARE_CLASS(
    sb::PushPullExecutive,
    "sb.PushPullExecutive",
    sb::AbstractExecutive
)

#endif // SB_EXECUTIVE_H
