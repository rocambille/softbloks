/*
Copyright (C) 2014 Bastien Oudot

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
#include <sb-core/sb-core.h>

using namespace std::placeholders;

class HelloSource : public sb::AbstractSource
{

    SB_DECLARE_OBJECT(HelloSource, "HelloSource")

public:

    std::string
    get_text
    (
    )
    const
    {
        return this->text;
    }

    void
    set_text
    (
        const std::string& value_
    )
    {
        this->text = value_;
    }

    static
    void
    construct
    (
        HelloSource* this_
    )
    {
        this_->text = "Hello World !!!";

        this_->register_property<std::string>(
            "HelloSource::text",
            sb::READ_WRITE,
            std::bind(
                &get_text, this_
            ),
            std::bind(
                &set_text, this_, _1
            )
        );
    }

private:

    std::string
    text;

};

class HelloFilter : public sb::AbstractFilter
{

    SB_DECLARE_OBJECT(HelloFilter, "HelloFilter")

public:

    int
    get_multiplier
    (
    )
    const
    {
        return this->multiplier;
    }

    void
    set_multiplier
    (
        const int& value_
    )
    {
        this->multiplier = value_;
    }

    static
    void
    construct
    (
        HelloFilter* this_
    )
    {
        this_->multiplier = 1;

        this_->register_property<int>(
            "HelloFilter::multiplier",
            sb::READ_WRITE,
            std::bind(
                &get_multiplier, this_
            ),
            std::bind(
                &set_multiplier, this_, _1
            )
        );
    }

private:

    int
    multiplier;

};

class HelloSink : public sb::AbstractSink
{

    SB_DECLARE_OBJECT(HelloSink, "HelloSink")

public:

    std::string
    get_text
    (
    )
    const
    {
        return this->text;
    }

    static
    void
    construct
    (
        HelloSink* this_
    )
    {
        this_->register_property<std::string>(
            "HelloSink::text",
            sb::READ_ONLY,
            std::bind(
                &get_text, this_
            ),
            nullptr
        );
    }

private:

    std::string
    text;

};

SB_DECLARE_MODULE(hellobloks)
{
    sb::register_object<HelloSource>();
    sb::register_object<HelloFilter>();
    sb::register_object<HelloSink>();
}
