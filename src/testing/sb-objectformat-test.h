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
#ifndef SB_OBJECTFORMAT_TEST_H
#define SB_OBJECTFORMAT_TEST_H

#include <gtest/gtest.h>

#include <sb-core/sb-coredefine.h>

namespace sb
{

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
        "sb.AbstractObject", "foo"
    },
    { // with a dummy property "bar" of type int in read/write mode
        SB_PROPERTY("bar", int, AccessRights::READ_WRITE)
    }
};

TEST_F(
    ObjectFormatTest,
    Equality
)
{
    EXPECT_TRUE(
        UNDEFINED_OBJECT_FORMAT == UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ANY_OBJECT_FORMAT == ANY_OBJECT_FORMAT
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
        UNDEFINED_OBJECT_FORMAT >> UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_FALSE(
        ANY_OBJECT_FORMAT >> UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format >> UNDEFINED_OBJECT_FORMAT
    );
}

TEST_F(
    ObjectFormatTest,
    ExtractAnyObjectFormat
)
{
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT >> ANY_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ANY_OBJECT_FORMAT >> ANY_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format >> ANY_OBJECT_FORMAT
    );
}

TEST_F(
    ObjectFormatTest,
    ExtractDummyFormat
)
{
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT >> ObjectFormatTest::foo_object_format
    );
    EXPECT_FALSE(
        ANY_OBJECT_FORMAT >> ObjectFormatTest::foo_object_format
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
        UNDEFINED_OBJECT_FORMAT << UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT << ANY_OBJECT_FORMAT
    );
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT << ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    InjectAnyObjectFormat
)
{
    EXPECT_FALSE(
        ANY_OBJECT_FORMAT << UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ANY_OBJECT_FORMAT << ANY_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ANY_OBJECT_FORMAT << ObjectFormatTest::foo_object_format
    );
}

TEST_F(
    ObjectFormatTest,
    InjectDummyFormat
)
{
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format << UNDEFINED_OBJECT_FORMAT
    );
    EXPECT_FALSE(
        ObjectFormatTest::foo_object_format << ANY_OBJECT_FORMAT
    );
    EXPECT_TRUE(
        ObjectFormatTest::foo_object_format << ObjectFormatTest::foo_object_format
    );
}

}

}

#endif // SB_OBJECTFORMAT_TEST_H
