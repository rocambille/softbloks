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

using namespace std::placeholders;

class HelloSource : public sb::AbstractSource
{

public:

    HelloSource
    (
    )
    {
        this->set_output_count(1);

        this->text = "Hello World!!!";

        this->get_output(0)->register_property<std::string>(
            this,
            "text",
            std::bind(
                &HelloSource::get_text, this
            ),
            std::bind(
                &HelloSource::set_text, this, _1
            )
        );
    }

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

        this->push_output(0);
    }

private:

    std::string
    text;

};

class HelloFilter : public sb::AbstractFilter
{

public:

    HelloFilter
    (
    )
    {
        this->set_input_count(1);

        this->set_input_format(
            0,
            {
                {
                    sb::get_type_name<sb::DataSet>()
                },
                {
                    SB_PROPERTY("text", std::string, sb::AccessRights::READ)
                }
            }
        );

        this->set_output_count(1);

        this->multiplier = 1;

        this->get_output(0)->register_property<int>(
            this,
            "multiplier",
            std::bind(
                &HelloFilter::get_multiplier, this
            ),
            std::bind(
                &HelloFilter::set_multiplier, this, _1
            )
        );

        this->get_output(0)->register_property<std::string>(
            this,
            "text",
            std::bind(
                &HelloFilter::get_text, this
            ),
            nullptr
        );
    }

    virtual
    void
    process
    (
    )
    override
    {
        // update this->text

        this->text = std::string();

        for(int i = 0; i < this->multiplier; ++i)
        {
            this->text.append(
                this->lock_input(0)->get<std::string>("text")
            ).append(
                "\n"
            );
        }

        // remove last "\n"

        if(this->text.size() > 0)
        {
            this->text.pop_back();
        }

        // push data

        this->push_output(0);
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

    std::string
    get_text
    (
    )
    const
    {
        return this->text;
    }

private:

    int
    multiplier;

    std::string
    text;

};

class HelloSink : public sb::AbstractSink
{

public:

    HelloSink
    (
    )
    {
        this->set_input_count(1);
        this->set_input_format(
            0,
            {
                {
                    sb::get_type_name<sb::DataSet>()
                },
                {
                    SB_PROPERTY("text", std::string, sb::AccessRights::READ)
                }
            }
        );

        this->register_property< std::function<void(void)> >(
            this,
            "observer",
            nullptr,
            std::bind(
                &HelloSink::register_observer, this, _1
            )
        );
    }

    virtual
    void
    process
    (
    )
    override
    {
        this->pull_input(0);

        for(auto notify : observers)
        {
            notify();
        }
    }

    void
    register_observer
    (
        const std::function<void(void)>& notify_
    )
    {
        this->observers.push_back(notify_);

        notify_();
    }

private:

    std::vector< std::function<void(void)> >
    observers;

};

SB_DECLARE_CLASS(
    HelloSource,
    "HelloSource",
    sb::AbstractSource
)
SB_DECLARE_CLASS(
    HelloFilter,
    "HelloFilter",
    sb::AbstractFilter
)
SB_DECLARE_CLASS(
    HelloSink,
    "HelloSink",
    sb::AbstractSink
)

SB_DECLARE_MODULE(hellobloks)
{
    sb::register_object<HelloSource>();
    sb::register_object<HelloFilter>();
    sb::register_object<HelloSink>();
}
