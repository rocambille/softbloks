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
#include "sb-core.h"

#include <QtWidgets>

class HelloSoft : public sb::AbstractSoft
{

    SB_DECLARE_OBJECT(HelloSoft, "HelloSoft")

public:

    QWidget*
    get_widget
    (
    )
    {
        QBoxLayout* layout = new QHBoxLayout;

        // create source's widget

        QLineEdit* line_edit = new QLineEdit(
            QString::fromStdString(
                this->source->get<std::string>(
                    "HelloSource::text"
                )
            )
        );

        layout->addWidget(line_edit, 1, Qt::AlignRight);

        layout->addWidget(new QLabel(" * "));

        // create filter's widget

        QSpinBox* spin_box = new QSpinBox;
        spin_box->setValue(
            this->filter->get<int>(
                "HelloFilter::multiplier"
            )
        );

        layout->addWidget(spin_box);

        layout->addWidget(new QLabel(" = "));

        // create sink's widget

        QLabel* label = new QLabel(
            QString::fromStdString(
                this->sink->get<std::string>(
                    "HelloSink::text"
                )
            )
        );

        layout->addWidget(label, 1, Qt::AlignLeft | Qt::AlignVCenter);

        // create and return container

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

    static
    void
    construct
    (
        HelloSoft* this_
    )
    {
        this_->source =
            sb::create<sb::AbstractBlok>("HelloSource");

        this_->filter =
            sb::create<sb::AbstractBlok>("HelloFilter");

        this_->sink =
            sb::create<sb::AbstractBlok>("HelloSink");

        this_->register_property<QWidget*>(
            "Qt5Widgets::main_view",
            sb::READ_ONLY,
            std::bind(&HelloSoft::get_widget, this_),
            nullptr
        );
    }

private:

    sb::SharedBlok
    source;

    sb::SharedBlok
    filter;

    sb::SharedBlok
    sink;

};

SB_DECLARE_MODULE(helloworld)
{
    sb::register_object<HelloSoft>();
}
