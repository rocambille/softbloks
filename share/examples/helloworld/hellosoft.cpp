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

#include <QtWidgets>

template<typename... Args>
struct SIGNAL
{
    template<typename C, typename R> 
    static
    auto
    FOR
    (
        R (C::*method_)(Args...)
    )
    -> decltype(method_)
    { 
        return method_;
    } 
};

class HelloSoft : public sb::AbstractSoft
{

public:

    HelloSoft
    (
    )
    {
        this->source = sb::create_unique_source("HelloSource");

        this->filter = sb::create_unique_filter("HelloFilter");

        this->filter->set_input(
            0,
            this->source->get_output(0)
        );

        this->sink = sb::create_unique_sink("HelloSink");

        this->sink->set_input(
            0,
            this->filter->get_output(0)
        );

        this->sink->process();

        this->register_property<QWidget*>(
            "Qt.mainview",
            std::bind(&HelloSoft::get_widget, this),
            nullptr
        );
    }

    QWidget*
    get_widget
    (
    )
    {
        QBoxLayout* layout = new QHBoxLayout;

        // create source's widget

        QLineEdit* line_edit = new QLineEdit(
            QString::fromStdString(
                this->source->get_output(0)->get<std::string>(
                    "text"
                )
            )
        );

        QObject::connect(
            line_edit, &QLineEdit :: textChanged,
            [this]
            (
                const QString& text_
            )
            {
                this->source->get_output(0)->set<std::string>(
                    "text",
                    text_.toStdString()
                );
            }
        );

        layout->addWidget(line_edit, 1, Qt::AlignRight);

        layout->addWidget(new QLabel(" * "));

        // create filter's widget

        QSpinBox* spin_box = new QSpinBox;
        spin_box->setValue(
            this->filter->get_output(0)->get<int>(
                "multiplier"
            )
        );

        QObject::connect(
            spin_box, SIGNAL<int>::FOR(&QSpinBox::valueChanged),
            [this]
            (
                int value_
            )
            {
                this->filter->get_output(0)->set<int>(
                    "multiplier",
                    value_
                );
            }
        );

        layout->addWidget(spin_box);

        layout->addWidget(new QLabel(" = "));

        // create sink's widget

        QLabel* label = new QLabel;

        this->sink->set< std::function<void(void)> >(
            "observer",
            [this, label]
            (
            )
            {
                label->setText(
                    QString::fromStdString(
                        this->sink->lock_input(0)->get<std::string>(
                            "text"
                        )
                    )
                );
            }
        );

        layout->addWidget(label, 1, Qt::AlignLeft | Qt::AlignVCenter);

        // create and return container

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

private:

    sb::UniqueSource
    source;

    sb::UniqueFilter
    filter;

    sb::UniqueSink
    sink;

};

SB_DECLARE_CLASS(
    HelloSoft,
    "HelloSoft",
    sb::AbstractSoft
)

SB_DECLARE_PROPERTIES(
    HelloSoft,
    SB_PROPERTY("Qt.mainview", QWidget*, sb::AccessRights::READ)
)

SB_DECLARE_MODULE(hellosoft)
{
    sb::register_object<HelloSoft>();
}
