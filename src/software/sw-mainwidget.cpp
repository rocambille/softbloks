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
#include "sw-mainwidget.h"
#include "sw-mainwidget-private.h"

using namespace sw;

MainWidget::MainWidget
(
)
{
    d_ptr = new MainWidgetPrivate(this);

    Q_D(MainWidget);

    this->setTabsClosable(true);

    // add home tab

    this->addTab(
        new QWidget,
        tr("H")
    );

    // add "new tab" button

    this->addTab(
        new QWidget,
        tr("+")
    );

    // for now, only permanent tabs were added:
    // loop to hide close buttons

    // for each index...
    for(int i(0); i < this->count(); ++i)
    {
        // ... and for each button position
        QVector<QTabBar::ButtonPosition> positions({
            QTabBar::LeftSide,
            QTabBar::RightSide
        });

        for(auto position : positions)
        {
            QWidget* button = this->tabBar()->tabButton(i, position);

            if(button)
            {
                // resize button with a null size:
                // same as QWidget::hide() except button's space is reclaimed

                button->resize(0, 0);
            }
        }
    }

    connect(
        this, &QTabWidget           :: currentChanged,
        d   , &MainWidgetPrivate    :: on_current_changed
    );

    connect(
        this, &QTabWidget           :: tabCloseRequested,
        d   , &MainWidgetPrivate    :: on_tab_close_requested
    );
}

MainWidgetPrivate::MainWidgetPrivate
(
    MainWidget* q_ptr_
):
    QObject (q_ptr_),
    q_ptr   (q_ptr_)
{
}

void
MainWidgetPrivate::on_current_changed
(
    int index_
)
{
    Q_Q(MainWidget);

    // if index_ corresponds to the last tab
    if(index_ == q->count() - 1)
    {
        // create new tab

        q->insertTab(
            index_,
            new QWidget,
            "o"
        );

        // no need to rerun this slot: disable signals
        q->blockSignals(true);

        // set new tab as current
        q->setCurrentIndex(index_);

        // reenable signals
        q->blockSignals(false);
    }
}

void
MainWidgetPrivate::on_tab_close_requested
(
    int index_
)
{
    Q_Q(MainWidget);

    // be careful to not trigger "new tab" button: disable signals
    q->blockSignals(true);

    // get widget
    QWidget* widget = q->widget(index_);

    // remove tab
    q->removeTab(index_);

    // widget is ready to be deleted
    delete widget;

    // if current index after remove corresponds to the last tab
    if(q->currentIndex() == q->count() - 1)
    {
        // change current index for a valid tab
        q->setCurrentIndex(
            q->currentIndex() - 1
        );
    }

    // reenable signals
    q->blockSignals(false);
}
