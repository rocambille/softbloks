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
#ifndef SB_COREDEFINE_TEST_H
#define SB_COREDEFINE_TEST_H

#include <gtest/gtest.h>

#include <sb-core/sb-coredefine.h>

namespace sb
{

namespace StaticPointerCastTest
{

class StaticPointerCastTest : public ::testing::Test
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
    StaticPointerCastTest,
    StaticPointerCast
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

        b = static_pointer_cast<B>(std::move(a));

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

namespace PropertyFormatTest
{

class PropertyFormatTest : public ::testing::Test
{

public:

    //virtual
    //void
    //SetUp
    //(
    //)
    //override
    //{
    //}

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

    static
    const PropertyFormat
    foo_property_format;

    static
    const PropertyFormat
    bar_property_format;

};

// create test formats

const PropertyFormat PropertyFormatTest::foo_property_format = {
    typeid(int), sb::READ_ONLY
};

const PropertyFormat PropertyFormatTest::bar_property_format = {
    typeid(std::string), sb::WRITE_ONLY
};

TEST_F(
    PropertyFormatTest,
    Equality
)
{
    EXPECT_TRUE(
        PropertyFormatTest::foo_property_format == PropertyFormatTest::foo_property_format
    );
    EXPECT_TRUE(
        PropertyFormatTest::bar_property_format == PropertyFormatTest::bar_property_format
    );
    EXPECT_FALSE(
        PropertyFormatTest::foo_property_format == PropertyFormatTest::bar_property_format
    );
}

}

namespace ObjectFormatTest
{

class ObjectFormatTest : public ::testing::Test
{

public:

    //virtual
    //void
    //SetUp
    //(
    //)
    //override
    //{
    //}

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

    static
    const ObjectFormat
    foo_object_format;

};

// create a test format
const ObjectFormat ObjectFormatTest::foo_object_format = {
    { // of type "foo" derived from sb::AbstractObject
        "sb::AbstractObject", "foo"
    },
    { // with a dummy property "bar" of type int in read/write mode
        {"bar", {typeid(int), sb::READ_WRITE}}
    }
};

TEST_F(
    ObjectFormatTest,
    Equality
)
{
    EXPECT_TRUE(
        undefined_object_format == undefined_object_format
    );
    EXPECT_TRUE(
        any_object_format == any_object_format
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format == ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    ExtractUndefinedFormat
)
{
    EXPECT_FALSE(
        undefined_object_format >> undefined_object_format
    );
    EXPECT_FALSE(
        any_object_format >> undefined_object_format
    );
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format >> undefined_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    ExtractAnyObjectFormat
)
{
    EXPECT_FALSE(
        undefined_object_format >> any_object_format
    );
    EXPECT_TRUE(
        any_object_format >> any_object_format
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format >> any_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    ExtractDummyFormat
)
{
    EXPECT_FALSE(
        undefined_object_format >> ObjectFormatTest::foo_object_format
    );
    EXPECT_FALSE(
        any_object_format >> ObjectFormatTest::foo_object_format
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format >> ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    InjectUndefinedFormat
)
{
    EXPECT_FALSE(
        undefined_object_format << undefined_object_format
    );
    EXPECT_FALSE(
        undefined_object_format << any_object_format
    );
    EXPECT_FALSE(
        undefined_object_format << ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    InjectAnyObjectFormat
)
{
    EXPECT_FALSE(
        any_object_format << undefined_object_format
    );
    EXPECT_TRUE(
        any_object_format << any_object_format
    );
    EXPECT_TRUE(
        any_object_format << ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    InjectDummyFormat
)
{
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format << undefined_object_format
    );
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format << any_object_format
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format << ObjectFormatTest::foo_object_format
    );
}

}

}

#endif // SB_COREDEFINE_TEST_H
