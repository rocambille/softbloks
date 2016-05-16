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
#ifndef SB_PROPERTYFORMAT_TEST_H
#define SB_PROPERTYFORMAT_TEST_H

#include <gtest/gtest.h>

#include <sb-core/sb-propertyformat.h>

namespace sb
{

namespace PropertyFormatTest
{

class PropertyFormatTest : public ::testing::Test
{

public:

    static
    void
    SetUpTestCase
    (
    )
    {
        // test each Property constructors compiles

        /*Property foo1(
            "foo",
            static_get_int,
            static_set_int
        );

        Property foo2(
            "foo",
            &PropertyFormatTest::get_int,
            &PropertyFormatTest::set_int
        );

        Property foo3(
            "foo",
            static_get_int
        );

        Property foo4(
            "foo",
            &PropertyFormatTest::get_int
        );

        Property foo5(
            "foo",
            static_set_int
        );

        Property foo6(
            "foo",
            &PropertyFormatTest::set_int
        );*/
    }

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

    int
    get_int
    (
    )
    const
    {
        return 0;
    }

    static
    int
    static_get_int
    (
        const PropertyFormatTest& /*this_*/
    )
    {
        return 0;
    }

    void
    set_int
    (
        const int& /*value_*/
    )
    {
    }

    static
    void
    static_set_int
    (
        PropertyFormatTest& /*this_*/,
        const int& /*value_*/
    )
    {
    }

};

TEST(
    PropertyFormatTest,
    Equality
)
{

    // create test formats

    PropertyFormat foo_property_format =
        make_property_format<int>(
            "foo", AccessRights::READ
        );

    PropertyFormat bar_property_format =
        make_property_format<std::string>(
            "bar", AccessRights::WRITE
        );

    // perform tests

    EXPECT_TRUE(
        foo_property_format == foo_property_format
    );
    EXPECT_TRUE(
        bar_property_format == bar_property_format
    );
    EXPECT_FALSE(
        foo_property_format == bar_property_format
    );
}

}

}

#endif // SB_PROPERTYFORMAT_TEST_H
