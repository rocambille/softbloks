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
#ifndef SB_STATIC_MOVE_CAST_TEST_H
#define SB_STATIC_MOVE_CAST_TEST_H

#include <gtest/gtest.h>

#include <sb-core/sb-coredefine.h>

namespace sb
{

namespace StaticMoveCastTest
{

class StaticMoveCastTest : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    override
    {
        this->destruction_progress = 0;
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

    int
    destruction_progress;

};

// dummy implementation of polymorphism

class A
{

public:

    A
    (
        int& destruction_progress_
    ):
        destruction_progress(destruction_progress_)
    {
    }

    virtual
    ~A
    (
    )
    {
        this->destruction_progress += 20;
    }

    int& destruction_progress;

};

class B : public A
{

public:

    B
    (
        int& destruction_progress_
    ):
        A(destruction_progress_)
    {
    }

    ~B
    (
    )
    {
        this->destruction_progress += 30;
    }

};

template<typename T>
using Unique = std::unique_ptr<T, std::function<void(A*)>>;

TEST_F(
    StaticMoveCastTest,
    StaticMoveCast
)
{
    {
        // create unique pointers to test

        Unique<A> a(
            new B(this->destruction_progress),
            [this]
            (
                A* ptr_
            )
            {
                delete ptr_;

                this->destruction_progress += 50;
            }
        );
        Unique<B> b;
        A* c = a.get();

        // assertions before cast

        EXPECT_NE( // a is not null
            nullptr,
            a.get()
        );
        EXPECT_EQ( // b is null
            nullptr,
            b.get()
        );
        EXPECT_EQ( // a points to c
            c,
            a.get()
        );

        // cast unique_ptr

        b = static_move_cast<B>(std::move(a));

        // assertions after cast

        EXPECT_EQ( // a is null
            nullptr,
            a.get()
        );
        EXPECT_NE( // b is not null
            nullptr,
            b.get()
        );
        EXPECT_EQ( // b points to c
            c,
            b.get()
        );
    }

    // check destruction was performed at 100% :
    // destructors were effectively called through
    // the deleter passed on construction

    EXPECT_EQ(
        100,
        this->destruction_progress
    );
}

}

}


#endif // SB_STATIC_MOVE_CAST_TEST_H
