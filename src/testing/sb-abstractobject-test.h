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
#ifndef SB_ABSTRACTOBJECT_TEST_H
#define SB_ABSTRACTOBJECT_TEST_H

#include <gtest/gtest.h>

#include <sb-core/sb-abstractobject.h>

#include <testing/sb-fixtures.h>

namespace sb
{

namespace AbstractObjectTest
{

// register_object

TYPED_TEST(
    TypedNoRegisteredObject,
    register_object
)
{
    EXPECT_TRUE(
        register_object<TypeParam>()
    ) << (
        "Failed to register "
    ) << (
        get_object_name<TypeParam>()
    );
}
TEST_F(
    NoRegisteredObject,
    register_registered_object
)
{
    ASSERT_TRUE(
        register_object<AbstractObject>()
    ) << (
        "Failed to register "
    ) << (
        get_object_name<AbstractObject>()
    );

    EXPECT_FALSE(
        register_object<AbstractObject>()
    ) << (
        "Registration of an already registered object did not failed"
    );
}

// get_registered_object_names

TEST_F(
    NoRegisteredObject,
    get_registered_object_names
)
{
    auto name_list = get_registered_object_names();

    EXPECT_EQ(
        0,
        name_list.size()
    ) << (
        "Found registered names while no object is registered"
    );
}
TYPED_TEST(
    OneRegisteredObject,
    get_registered_object_names
)
{
    auto name_list = get_registered_object_names();

    EXPECT_TRUE(
        std::find(
            name_list.begin(),
            name_list.end(),
            get_object_name<TypeParam>()
        ) !=  name_list.end()
    ) << (
        "Registered name "
    ) << (
        get_object_name<TypeParam>()
    ) << (
        "can not be found"
    );
}

// create_unique

TEST_F(
    NoRegisteredObject,
    create_unique
)
{
    EXPECT_EQ(
        nullptr,
        create_unique_object(
            "foo"
        ).get()
    ) << (
        "Creation of a non-registered object returned a valid pointer"
    );
}
TYPED_TEST(
    OneRegisteredObject,
    create_unique
)
{
    EXPECT_NE(
        nullptr,
        create_unique<TypeParam>(
            get_object_name<TypeParam>()
        ).get()
    ) << (
        "Failed to create unique "
    ) << (
        get_object_name<TypeParam>()
    );
}

// create_shared

TEST_F(
    NoRegisteredObject,
    create_shared
)
{
    EXPECT_EQ(
        nullptr,
        create_shared_object(
            "foo"
        ).get()
    ) << (
        "Creation of a non-registered object returned a valid pointer"
    );
}
TYPED_TEST(
    OneRegisteredObject,
    create_shared
)
{
    EXPECT_NE(
        nullptr,
        create_shared<TypeParam>(
            get_object_name<TypeParam>()
        )
    ) << (
        "Failed to create shared "
    ) << (
        get_object_name<TypeParam>()
    );
}

// get_object_format

TEST_F(
    NoRegisteredObject,
    get_object_format
)
{
    EXPECT_TRUE(
        get_object_format("foo") == undefined_object_format
    );
}
class Object : public AbstractObject
{
};
TYPED_TEST(
    OneCreatedObject,
    get_object_format
)
{
    Object object;
    auto object_format = undefined_object_format;

    ASSERT_NO_THROW(
        object_format = get_object_format(
            get_object_name<TypeParam>()
        )
    );

    EXPECT_TRUE(
        this->instance->get_instance_format() >> object_format
    );
}

}

}

#endif // SB_ABSTRACTOBJECT_TEST_H
