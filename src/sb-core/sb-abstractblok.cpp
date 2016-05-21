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
#include <sb-core/sb-abstractblok.h>

#include <sb-core/sb-abstractblok-private.h>

#include <algorithm>

#include <sb-core/sb-abstractdata-private.h>
#include <sb-core/sb-abstractexecutive-private.h>
#include <sb-core/sb-executive.h>

using namespace sb;

AbstractBlok::AbstractBlok
(
)
{
    sb::register_object<PushPullExecutive>();

    this->d_ptr = new Private(this);

    this->use_executive(
        get_type_name<PushPullExecutive>()
    );
}

AbstractBlok::~AbstractBlok
(
)
{
    for(Index i = 0; i < d_ptr->inputs.size(); ++i)
    {
        d_ptr->unlink_input(i);
    }

    for(auto output : d_ptr->outputs)
    {
        AbstractData::Private::from(
            output
        )->source_blok = SB_NULLPTR;
    }

    // d_ptr->executive points to this blok:
    // ensure it is destroyed first

    d_ptr->executive.reset();

    delete d_ptr;
}

Size
AbstractBlok::get_input_count
(
)
const
{
    return d_ptr->inputs.size();
}

Size
AbstractBlok::get_output_count
(
)
const
{
    return d_ptr->outputs.size();
}

void
AbstractBlok::use_executive
(
    const std::string& name_
)
{
    d_ptr->executive = create_unique_executive(name_);

    if(!d_ptr->executive)
    {
        throw std::invalid_argument(
            "fatal: AbstractBlok::use_executive(invalid executive name)"
        );
    }

    AbstractExecutive::Private::from(
        d_ptr->executive
    )->blok = this;
}

void
AbstractBlok::pull_input
(
    Index index_
)
{
    // AbstractBlok::Private::lock_input calls this method:
    // don't call it here or it will cause infinite recursion

    auto input_d_ptr = AbstractData::Private::from(
        d_ptr->inputs.at(index_).lock()
    );

    AbstractBlok::Private::from(
        input_d_ptr->source_blok
    )->executive->on_output_pulled(
        input_d_ptr->source_index
    );
}

void
AbstractBlok::push_output
(
    Index index_
)
{
    auto output_d_ptr = AbstractData::Private::from(
        d_ptr->outputs.at(index_)
    );

    for(auto follower : output_d_ptr->followers)
    {
        AbstractBlok::Private::from(
            Unmapper::blok(follower)
        )->executive->on_input_pushed(
            Unmapper::input_index(follower)
        );
    }
}

void
AbstractBlok::init
(
    AbstractBlok* this_,
    const ObjectFormatSequence& inputs_formats_,
    const StringSequence& output_type_names_
)
{
    auto d_ptr = AbstractBlok::Private::from(
        this_
    );

    d_ptr->set_inputs_formats(
        inputs_formats_
    );
    d_ptr->set_outputs_type_names(
        output_type_names_
    );
}

AbstractBlok::Private::Private
(
    AbstractBlok* q_ptr_
):
    q_ptr(q_ptr_)
{
}

void
AbstractBlok::Private::set_inputs_formats
(
    const ObjectFormatSequence& value_
)
{
    this->inputs_formats = value_;

    this->inputs.resize(
        this->inputs_formats.size()
    );
}

void
AbstractBlok::Private::set_outputs_type_names
(
    const StringSequence& value_
)
{
    this->outputs_formats.resize(
        value_.size()
    );
    this->outputs.resize(
        value_.size()
    );

    for(Index i(0); i < value_.size(); ++i)
    {
        this->outputs_formats[i] = get_object_format(
            value_[i]
        );

        SharedData data = create_shared_data(
            value_[i]
        );

        auto data_d_ptr = AbstractData::Private::from(
            data
        );

        data_d_ptr->source_blok = q_ptr;
        data_d_ptr->source_index = i;

        this->outputs[i] = data;
    }
}

SharedData
AbstractBlok::Private::lock_input
(
    Index index_
)
const
{
    q_ptr->pull_input(index_);

    return this->inputs.at(index_).lock();
}

bool
AbstractBlok::Private::set_input
(
    Index index_,
    const SharedData& value_
)
{
    bool ok = false;

    if(
        value_ == SB_NULLPTR ||
        value_->get_format().includes(
            this->inputs_formats.at(index_)
        )
    )
    {
        ok = true;

        this->unlink_input(index_);

        this->inputs[index_] = value_;

        if(value_)
        {
            // register this blok as a follower

            AbstractData::Private::from(
                value_
            )->followers.emplace(
                q_ptr, index_
            );
        }
    }

    return ok;
}

void
AbstractBlok::Private::unlink_input
(
    Index index_
)
{
    auto input = this->inputs[index_].lock();

    if(input)
    {
        // unregister this blok from previous input's followers

        auto input_d_ptr = AbstractData::Private::from(
            input
        );

        auto erase_candidates = input_d_ptr->followers.equal_range(
            q_ptr
        );

        auto candidate = erase_candidates.first;

        while(candidate != erase_candidates.second)
        {
            if(Unmapper::input_index(*candidate) == index_)
            {
                candidate = input_d_ptr->followers.erase(candidate);
            }
            else
            {
                ++candidate;
            }
        }
    }
}

AbstractBlok::Private*
AbstractBlok::Private::from
(
    const AbstractBlok* this_
)
{
    return this_->d_ptr;
}

AbstractBlok::Private*
AbstractBlok::Private::from
(
    const UniqueBlok& this_
)
{
    return this_->d_ptr;
}

bool
sb::connect
(
    AbstractBlok* left_,
    Index left_index_,
    AbstractBlok* right_,
    Index right_index_
)
{
    return
        AbstractBlok::Private::from(
            right_
        )->set_input(
            right_index_,
            AbstractBlok::Private::from(
                left_
            )->outputs[
                left_index_
            ]
        );
}
