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
#ifndef SW_MAINWIDGET_PRIVATE_H
#define SW_MAINWIDGET_PRIVATE_H

#include "sw-mainwidget.h"

#include <QtSvg>
#include <QtWidgets>

namespace sw
{

class MainWidgetPrivate : public QObject
{

    Q_OBJECT

    Q_DECLARE_PUBLIC(MainWidget)

public:

    MainWidgetPrivate
    (
        MainWidget* q_ptr_
    );

    void
    on_current_changed
    (
        int index_
    );

    void
    on_tab_close_requested
    (
        int index_
    );

    QWidget*
    create_home_widget
    (
    );

    QWidget*
    create_get_started_widget
    (
    );

    QWidget*
    create_options_widget
    (
    );

    QWidget*
    create_about_widget
    (
    );

    QWidget*
    create_chooser
    (
    );

public:

    MainWidget*
    q_ptr;

    QWidget*
    home_widget;

    QListWidget*
    module_list_widget;

    QMap<QWidget*, sb::SharedObject>
    widget_to_soft_map;

};

}

#endif // SW_MAINWIDGET_PRIVATE_H
