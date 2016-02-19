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
        d->home_widget,
        tr("H")
    );

    // add "new tab" button

    this->addTab(
        new QWidget,
        "+"
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

void
MainWidget::read_settings
(
    const QSettings& settings_
)
{
    Q_D(MainWidget);

    this->restoreGeometry(
        settings_.value(
            "MainWidget/geometry"
        ).toByteArray()
    );

    QStringList module_paths = settings_.value(
        "Global/module_paths"
    ).toStringList();

    d->module_list_widget->clear();

    foreach(QString module_path, module_paths)
    {
        d->module_list_widget->addItem(module_path);
    }
}

void
MainWidget::write_settings
(
    QSettings& settings_
)
const
{
    Q_D(const MainWidget);

    settings_.setValue(
        "MainWidget/geometry",
        this->saveGeometry()
    );

    QStringList module_paths;

    for(int i(0); i < d->module_list_widget->count(); ++i)
    {
        module_paths.append(
            d->module_list_widget->item(i)->text()
        );
    }

    settings_.setValue(
        "Global/module_paths",
        module_paths
    );
}

MainWidgetPrivate::MainWidgetPrivate
(
    MainWidget* q_ptr_
):
    QObject (q_ptr_),
    q_ptr   (q_ptr_)
{
    this->home_widget = this->create_home_widget();
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
            this->create_chooser(),
            tr("Choose...")
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

QWidget*
MainWidgetPrivate::create_home_widget
(
)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

        // declare a stacked layout to display pages

        QStackedLayout* stacked_layout = new QStackedLayout;

    layout->addLayout(stacked_layout, 1);

        // declare a sub layout as menu to select the current page

        QHBoxLayout* sub_layout = new QHBoxLayout;
        sub_layout->setContentsMargins(0, 0, 0, 0);

        QWidget* sub_widget = new QWidget;
        sub_widget->setLayout(sub_layout);
        sub_widget->setStyleSheet(
            R"(
                background: #dfdfdf;
            )"
        );

    layout->addWidget(sub_widget);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    QButtonGroup* button_group = new QButtonGroup(widget);

    // declare pages (don't use a map to keep add order)

    QVector< QPair<QString, QWidget*> > pages;

    pages.append({
        tr("Get started"),
        this->create_get_started_widget()
    });
    pages.append({
        tr("Options"),
        this->create_options_widget()
    });
    pages.append({
        tr("About"),
        this->create_about_widget()
    });

    // for each page
    foreach(auto page, pages)
    {
        // add to the stacked layout

        int index = stacked_layout->addWidget(page.second);

        // create a button entry

        QPushButton* button = new QPushButton(page.first);
        button->setStyleSheet(
            R"(
                QPushButton
                {
                    background: #dfdfdf;
                    border: none;
                    color: #0000EE;
                    height: 7em;
                }
                QPushButton:hover
                {
                    text-decoration: underline;
                }
                QPushButton:checked
                {
                    background: #bdbdbd;
                }
            )"
        );
        button->setCheckable(true);

        button_group->addButton(button);
        sub_layout->addWidget(button);

        // connect button to stacked layout

        connect(
            button, &QPushButton :: toggled,
            [index, stacked_layout]
            (
                bool checked_
            )
            {
                if(checked_)
                {
                    stacked_layout->setCurrentIndex(index);
                }
            }
        );
    }

    button_group->buttons().first()->setChecked(true);

    return widget;
}

QWidget*
MainWidgetPrivate::create_get_started_widget
(
)
{
    QTextEdit* widget = new QTextEdit;
    widget->setReadOnly(true);
    widget->setPlainText(
        tr(
            "Software is a an integration tool for Qt-based softs.\n\n"
            "Paths to modules declaring softs can be added in the options "
            "below.\n\n"
            "You can see available softs by clicking the button \"+\" above. "
            "Double-click on a soft name to run it."
        )
    );

    return widget;
}

QWidget*
MainWidgetPrivate::create_options_widget
(
)
{
    Q_Q(MainWidget);

    QVBoxLayout* layout = new QVBoxLayout;

        QToolBar* tool_bar = new QToolBar;

    layout->addWidget(
        tool_bar, 0, Qt::AlignCenter
    );

        this->module_list_widget = new QListWidget;

    layout->addWidget(
        this->module_list_widget, 1
    );

        QLabel* label = new QLabel(
            tr("(Need to restart)")
        );
        label->setVisible(false);

    layout->addWidget(
        label, 0, Qt::AlignCenter
    );

    QAction* action;
    action = tool_bar->addAction(
        "+"
    );
    connect(
        action, &QAction :: triggered,
        [this, q, label]
        (
        )
        {
            QString last_dir = QString();

            if(this->module_list_widget->count() > 0)
            {
                last_dir = this->module_list_widget->item(
                    this->module_list_widget->count() - 1
                )->text();
            }

            QString new_dir = QFileDialog::getExistingDirectory(
                q,
                QString(),
                last_dir
            );

            if(!new_dir.isEmpty())
            {
                this->module_list_widget->addItem(
                    new_dir
                );

                label->setVisible(true);
            }
        }
    );
    action = tool_bar->addAction(
        "-"
    );
    connect(
        action, &QAction :: triggered,
        [this, label]
        (
        )
        {
            if(this->module_list_widget->count() > 0)
            {
                delete this->module_list_widget->takeItem(
                    this->module_list_widget->currentRow()
                );

                label->setVisible(true);
            }
        }
    );

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

class SvgWidget : public QSvgWidget
{

public:

    SvgWidget
    (
        const QString& file_,
        QWidget* parent_ = nullptr
    ):
        QSvgWidget(file_, parent_)
    {
        this->initial_default_size = this->renderer()->defaultSize();
    }

protected:

    virtual
    void
    resizeEvent
    (
        QResizeEvent* event_
    )
    override
    {
        QSize view_box_size = event_->size().scaled(
            this->initial_default_size,
            Qt::KeepAspectRatioByExpanding
        );

        this->renderer()->setViewBox(
            QRect(
                QPoint(
                    this->initial_default_size.width() -
                        view_box_size.width(),
                    (
                        this->initial_default_size.height() -
                            view_box_size.height()
                    )/2
                ),
                view_box_size
            )
        );

        QSvgWidget::resizeEvent(event_);
    }

private:

    QSize
    initial_default_size;

};

QWidget*
MainWidgetPrivate::create_about_widget
(
)
{
    QHBoxLayout* layout = new QHBoxLayout;

        QSvgWidget* svg_widget = new SvgWidget(":/software.svg");

    layout->addWidget(
        svg_widget
    );

    layout->addWidget(
        new QLabel(
            QString("%1 v%2").
                arg(QApplication::applicationName()).
                arg(QApplication::applicationVersion())
        )
    );

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

QWidget*
MainWidgetPrivate::create_chooser
(
)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);

    QListWidget* list_widget = new QListWidget;

    auto names = sb::get_registered_object_names(
        {
            {
                sb::get_object_name<sb::AbstractSoft>()
            },
            {
                {"Qt.mainview", {typeid(QWidget*), sb::READ_ONLY}}
            }
        }
    );

    for(auto name : names)
    {
        list_widget->addItem(
            QString::fromStdString(name)
        );
    }

    connect(
        list_widget, &QListWidget :: itemDoubleClicked,
        [this, layout, list_widget]
        (
            QListWidgetItem* item_
        )
        {
            // remove list_widget from the layout

            list_widget->hide();

            layout->removeWidget(list_widget);

            list_widget->deleteLater();

            // replace it with soft's widget

            auto soft = sb::create_shared_object(
                item_->text().toStdString()
            );

            QWidget* widget = soft->get<QWidget*>("Qt.mainview");

            layout->addWidget(
                widget
            );

            // manage widget/soft storing

            widget_to_soft_map.insert(
                widget,
                soft
            );

            connect(
                widget, &QObject :: destroyed,
                [this]
                (
                    QObject* object_
                )
                {
                    this->widget_to_soft_map.remove(
                        qobject_cast<QWidget*>(object_)
                    );
                }
            );
        }
    );

    layout->addWidget(list_widget);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
