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
#include "sw-application.h"
#include "sw-application-private.h"

using namespace sw;

Application::Application
(
    int& argc_,
    char** argv_
):
    QApplication(argc_, argv_)
{
    this->setApplicationName(
        "Software"
    );
    this->setApplicationVersion(
        QString("%1.%2 20%3.%4.%5").
            arg(sb::VERSION_MAJOR).
            arg(sb::VERSION_MINOR).
            arg(sb::BUILD_YEAR).
            arg(sb::BUILD_MONTH).
            arg(sb::BUILD_DAY)
    );
    this->setWindowIcon(
        QIcon(":/software.svg")
    );

    d_ptr = new ApplicationPrivate(this);

    Q_D(Application);

    d->read_settings();

    connect(
        this, &QApplication :: lastWindowClosed,
        [d]
        (
        )
        {
            d->write_settings();
        }
    );

    d->main_widget.show();
}

Application::~Application
(
)
{
    // make sure to forget dynamically loaded factories,
    // before libraries are unloaded

    sb::unregister_all_objects();
}

ApplicationPrivate::ApplicationPrivate
(
    Application* q_ptr_
):
    QObject (q_ptr_),
    q_ptr   (q_ptr_),
    settings("software.ini", QSettings::IniFormat)
{
}

void
ApplicationPrivate::read_settings
(
)
{
    QStringList module_paths = this->settings.value(
        "Global/module_paths"
    ).toStringList();

    foreach(QString module_path, module_paths)
    {
        QDir dir(module_path);

        foreach(QString entry, dir.entryList(QDir::Files))
        {
            QLibrary library(dir.filePath(entry));

            QFunctionPointer sb_main = (
                library.resolve("sb_main")
            );

            if(sb_main)
            {
                sb_main();
            }
        }
    }

    this->main_widget.read_settings(
        this->settings
    );
}

void
ApplicationPrivate::write_settings
(
)
{
    this->main_widget.write_settings(
        this->settings
    );
}
