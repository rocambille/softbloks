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
        get_type_name<TypeParam>()
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
        get_type_name<AbstractObject>()
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
    auto name_sequence = get_registered_object_names();

    EXPECT_EQ(
        0,
        name_sequence.size()
    ) << (
        "Found registered names while no objects are registered"
    );
}
TYPED_TEST(
    OneRegisteredObject,
    get_registered_object_names
)
{
    auto name_sequence = get_registered_object_names();

    EXPECT_TRUE(
        std::find(
            name_sequence.begin(),
            name_sequence.end(),
            get_type_name<TypeParam>()
        ) !=  name_sequence.end()
    ) << (
        "Registered name "
    ) << (
        get_type_name<TypeParam>()
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
        SB_NULLPTR,
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
        SB_NULLPTR,
        create_unique<TypeParam>(
            get_type_name<TypeParam>()
        ).get()
    ) << (
        "Failed to create unique "
    ) << (
        get_type_name<TypeParam>()
    );
}

// create_shared

TEST_F(
    NoRegisteredObject,
    create_shared
)
{
    EXPECT_EQ(
        SB_NULLPTR,
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
        SB_NULLPTR,
        create_shared<TypeParam>(
            get_type_name<TypeParam>()
        )
    ) << (
        "Failed to create shared "
    ) << (
        get_type_name<TypeParam>()
    );
}

// get_object_format

TEST_F(
    NoRegisteredObject,
    get_object_format
)
{
    EXPECT_TRUE(
        UNDEFINED_OBJECT_FORMAT == get_object_format("foo")
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
    auto object_format = UNDEFINED_OBJECT_FORMAT;

    ASSERT_NO_THROW(
        object_format = get_object_format(
            get_type_name<TypeParam>()
        )
    );

    EXPECT_TRUE(
        this->instance->get_instance_format().includes(object_format)
    );
}

}

}

#endif // SB_ABSTRACTOBJECT_TEST_H
