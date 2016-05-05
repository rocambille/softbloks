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

#include <sb-core/sb-coredefine.h>

namespace sb
{

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
    typeid(int), sb::AccessRights::READ
};

const PropertyFormat PropertyFormatTest::bar_property_format = {
    typeid(std::string), sb::AccessRights::WRITE
};

TEST_F(
    PropertyFormatTest,
    Equality
)
{
    EXPECT_TRUE(
        PropertyFormatTest::foo_property_format ==
        PropertyFormatTest::foo_property_format
    );
    EXPECT_TRUE(
        PropertyFormatTest::bar_property_format ==
        PropertyFormatTest::bar_property_format
    );
    EXPECT_FALSE(
        PropertyFormatTest::foo_property_format ==
        PropertyFormatTest::bar_property_format
    );
}

}

}

#endif // SB_PROPERTYFORMAT_TEST_H
