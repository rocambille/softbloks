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
#include <sb-core/sb-core.h>

class HelloSource : public sb::AbstractSource
{

    SB_NAME("HelloSource")

    SB_PROPERTIES({
        "text",
        &HelloSource::get_text,
        &HelloSource::set_text
    })

    SB_OUTPUTS_TYPES(
        std::string
    )

public:

    virtual
    void
    init
    (
    )
    SB_OVERRIDE
    {
        this->set_text("Hello World!!!");
    }

    virtual
    void
    process
    (
    )
    SB_OVERRIDE
    {
        this->push_output();
    }

    std::string
    get_text
    (
    )
    const
    {
        return this->get_output()->get<std::string>("value");
    }

    void
    set_text
    (
        const std::string& value_
    )
    {
        this->get_output()->set("value", value_);

        this->process();
    }

};

class HelloFilter : public sb::AbstractFilter
{

    SB_NAME("HelloFilter")

    SB_PROPERTIES({
        "multiplier",
        &HelloFilter::get_multiplier,
        &HelloFilter::set_multiplier
    })

    SB_INPUTS_FORMATS(
        HelloSource::get_outputs_formats()
    )

    SB_OUTPUTS_TYPES(
        std::string
    )

public:

    HelloFilter
    (
    ):
        multiplier(1)
    {
    }

    virtual
    void
    process
    (
    )
    SB_OVERRIDE
    {
        // compute output text

        std::string input_text =
            this->lock_input()->get<std::string>("value") + '\n';

        std::string output_text = input_text;

        for(int i = 1; i < this->multiplier; ++i)
        {
            output_text.append(input_text);
        }

        // remove last '\n'

        output_text.pop_back();

        // update output data

        this->get_output()->set("value", output_text);

        this->push_output();
    }

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

        this->process();
    }

private:

    int
    multiplier;

};

class HelloSink : public sb::AbstractSink
{

    SB_NAME("HelloSink")

    SB_PROPERTIES({
        "text",
        &HelloSink::get_text
    },{
        "notifier",
        &HelloSink::set_notifier
    })

    SB_INPUTS_FORMATS(
        HelloFilter::get_outputs_formats()
    )

public:

    virtual
    void
    process
    (
    )
    SB_OVERRIDE
    {
        if(notifier)
        {
            notifier();
        }
    }

    std::string
    get_text
    (
    )
    const
    {
        return this->lock_input()->get<std::string>("value");
    }

    void
    set_notifier
    (
        const std::function<void(void)>& value_
    )
    {
        this->notifier = value_;
    }

private:

    std::function<void(void)>
    notifier;

};

SB_MODULE(hellobloks)
{
    sb::register_data<std::string>();

    sb::register_object<HelloSource>();
    sb::register_object<HelloFilter>();
    sb::register_object<HelloSink>();
}
