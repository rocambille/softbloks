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
struct OVERLOAD
{

    template<typename C, typename R>
    static
    SB_CONSTEXPR_FUNCTION
    auto
    OF
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

    SB_NAME("HelloSoft")

    SB_PROPERTIES({
        "Qt.mainview",
        &HelloSoft::get_qt_mainview
    })

public:

    HelloSoft
    (
    )
    {
        this->source = sb::create_unique_blok(
            "HelloSource"
        );
        this->filter = sb::create_unique_blok(
            "HelloFilter"
        );
        this->sink = sb::create_unique_blok(
            "HelloSink"
        );

        sb::connect(this->source, this->filter);
        sb::connect(this->filter, this->sink);
    }

    QWidget*
    get_qt_mainview
    (
    )
    const
    {
        QBoxLayout* layout = new QHBoxLayout;

        // create source's widget

        QLineEdit* line_edit = new QLineEdit(
            QString::fromStdString(
                this->source->get<std::string>(
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
                this->source->set(
                    "text",
                    text_.toStdString()
                );
            }
        );

        layout->addWidget(line_edit, 1, Qt::AlignRight);

        layout->addWidget(new QLabel(" * "));

        // create filter's widget

        QSpinBox* spin_box = new QSpinBox;
        spin_box->setRange(1, 40);
        spin_box->setValue(
            this->filter->get<int>(
                "multiplier"
            )
        );

        QObject::connect(
            spin_box, OVERLOAD<int>::OF(&QSpinBox::valueChanged),
            [this]
            (
                int value_
            )
            {
                this->filter->set(
                    "multiplier",
                    value_
                );
            }
        );

        layout->addWidget(spin_box);

        layout->addWidget(new QLabel(" = "));

        // create sink's widget

        QLabel* label = new QLabel;

        std::function<void(void)> label_updater =
            [this, label]
            (
            )
            {
                label->setText(
                    QString::fromStdString(
                        this->sink->get<std::string>(
                            "text"
                        )
                    )
                );
            };

        this->sink->set(
            "notifier",
            label_updater
        );

        label_updater();

        layout->addWidget(label, 1, Qt::AlignLeft | Qt::AlignVCenter);

        // create and return container

        QWidget* widget = new QWidget;
        widget->setLayout(layout);

        return widget;
    }

private:

    sb::UniqueBlok
    source;

    sb::UniqueBlok
    filter;

    sb::UniqueBlok
    sink;

};

SB_MODULE(hellosoft)
{
    sb::register_object<HelloSoft>();
}
