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
#ifndef SB_FIXTURES_H
#define SB_FIXTURES_H

#include <gtest/gtest.h>

#include <sb-core/sb-core.h>

namespace sb
{

using InstantiableTypes = ::testing::Types<
    AbstractBlok,
    AbstractData,
    AbstractFilter,
    AbstractObject,
    AbstractSink,
    AbstractSoft,
    AbstractSource,
    DataSet,
    PushExecutive,
    PullExecutive,
    PushPullExecutive,
    AbstractObject
>;

class NoRegisteredObject : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    override
    {
        unregister_all_objects();
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

};

template<typename T>
class TypedNoRegisteredObject : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    override
    {
        unregister_all_objects();
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

};

TYPED_TEST_CASE(TypedNoRegisteredObject, InstantiableTypes);

template<typename T>
class OneRegisteredObject : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    override
    {
        unregister_all_objects();

        register_object<T>();
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

};

TYPED_TEST_CASE(OneRegisteredObject, InstantiableTypes);

template<typename T>
class OneCreatedObject : public ::testing::Test
{

public:

    virtual
    void
    SetUp
    (
    )
    override
    {
        unregister_all_objects();

        register_object<T>();

        this->instance = create_unique<T>(
            get_type_name<T>()
        );
    }

    //virtual
    //void
    //TearDown
    //(
    //)
    //override
    //{
    //}

    Unique<T> instance;

};

TYPED_TEST_CASE(OneCreatedObject, InstantiableTypes);

}

#endif // SB_FIXTURES_H
