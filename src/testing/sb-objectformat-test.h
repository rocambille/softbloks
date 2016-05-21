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

#include <sb-core/sb-core.h>

namespace sb
{

namespace ObjectFormatTest
{

class ObjectFormatTest : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    SB_OVERRIDE
    {
        // create a test format
        foo_object_format =
            // use AbstractObject's format as a base
            ObjectFormat(ANY_OBJECT_FORMAT) <<
                // add a type name "foo"
                "foo" <<
                // and a dummy property "bar"
                sb::make_property_format<int>(
                    "bar", AccessRights::READ_WRITE
                );
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //SB_OVERRIDE
    //{
    //}

    ObjectFormat
    foo_object_format;

};

TEST_F(
    ObjectFormatTest,
    Inclusion
)
{
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT.includes(UNDEFINED_OBJECT_FORMAT)
    );
    EXPECT_FALSE(
        UNDEFINED_OBJECT_FORMAT.includes(ANY_OBJECT_FORMAT)
    );
    EXPECT_FALSE(
        ANY_OBJECT_FORMAT.includes(UNDEFINED_OBJECT_FORMAT)
    );
    EXPECT_TRUE(
        ANY_OBJECT_FORMAT.includes(ANY_OBJECT_FORMAT)
    );
    EXPECT_FALSE(
        ANY_OBJECT_FORMAT.includes(foo_object_format)
    );
    EXPECT_TRUE(
        foo_object_format.includes(ANY_OBJECT_FORMAT)
    );
    EXPECT_TRUE(
        foo_object_format.includes(foo_object_format)
    );
}

}

}

#endif // SB_OBJECTFORMAT_TEST_H
