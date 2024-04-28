/* MainWindow.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileINfo>
#include <QFileSystemModel>
#include <QList>
#include <QListView>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QObject>
#include <QPlainTextEdit>
#include <QProcess>
#include <QRect>
#include <QResizeEvent>
#include <QScreen>
#include <QStringListModel>
#include <QTextStream>
#include <QWidget>

#include <functional>
#include <iostream>
#include <memory>

#include "IOCore/Exception.hpp"

#include "EntityEditor.hpp"
#include "MainWindow.hpp"
#include "fmt/format.h"
#include "ui_MainWindow.h"

namespace ResourceEditor {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , filesystem_model(std::make_unique<QFileSystemModel>())
{
	this->ui->setupUi(this);

	connect(
	    this->ui->actionOpen,
	    &QAction::triggered,
	    this,
	    &MainWindow::onclick_open_directory
	);
	this->read_directory("/Users/sdavid");
}

MainWindow::~MainWindow() {}

void MainWindow::showEvent(QShowEvent* event)
{
	auto screen_rect = QApplication::primaryScreen()->availableGeometry();
	auto center_point = QPoint(
	    screen_rect.x() + (screen_rect.width() / 2),
	    screen_rect.y() + (screen_rect.height() / 2)
	);

	center_point.setX(center_point.x() - (this->width() / 2));
	center_point.setY(center_point.y() - (this->height() / 2));

	this->move(center_point);
}
void MainWindow::onclick_open_directory()
{
	auto current_directory = QDir::current().dirName();
	auto user_selection = QFileDialog::getExistingDirectory(
	    this,
	    QWidget::tr("Open Directory"),
	    current_directory,
	    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	this->read_directory(user_selection);
};

void MainWindow::read_directory(const QString& directory)
{
	auto model_index = this->filesystem_model->setRootPath(directory);

	this->ui->fsTreeView->setModel(this->filesystem_model.get());
	this->ui->fsTreeView->setRootIndex(model_index);

	this->ui->fsTreeView->hideColumn(1);
	this->ui->fsTreeView->hideColumn(2);
	this->ui->fsTreeView->hideColumn(3);
	this->ui->fsTreeView->hideColumn(4);
	this->ui->fsTreeView->hideColumn(5);

	connect(
	    this->ui->fsTreeView,
	    &QTreeView::doubleClicked,
	    this,
	    &MainWindow::on_file_click
	);
}

void MainWindow::on_file_click(const QModelIndex& index)
{
	auto path = filesystem_model->filePath(index);
	auto found_document_iter = open_documents.find(path);
	QMdiSubWindow* subwindow = nullptr;

	QFileInfo path_info(path);
	if (path_info.isDir()) {
		this->read_directory(path);
		return;
	}

	if (found_document_iter != open_documents.end()) {
		this->statusBar()->showMessage(
		    tr("Document was already loaded"), 1000
		);

		subwindow = found_document_iter->second;
	} else {
		this->statusBar()->showMessage(tr("Opened New Document"), 1000);
		auto text_widget = new QPlainTextEdit(this->ui->mdiArea);
		subwindow = this->ui->mdiArea->addSubWindow(text_widget);

		QFile file(path);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream file_text(&file);
			text_widget->setPlainText(file_text.readAll());
			file.close();
		}
		this->open_documents[path] = subwindow;
	}

	////this->ui->mdiArea->setActiveSubWindow(subwindow);
	subwindow->show();
}

/*
void MainWindow::load_file(const QString& file_path)
{
        QMdiSubWindow* document_to_open =
find_document_window(file_path);

        if (document_to_open) {
                statusBar()->showMessage(
                    tr("Document was already loaded"), 1000
                );
        } else {
                document_to_open = create_document_window(file_path);

                if (document_to_open) {
                        statusBar()->showMessage(tr("File loaded"),
1000); } else { throw IOCore::Exception("The file could not be loaded!");
                }
        }

        ui->mdiArea->setActiveSubWindow(document_to_open);
        document_to_open->show();
}

auto MainWindow::find_document_window(const QString& file_path) ->
QMdiSubWindow*
{
        QString absolute_path = QFileInfo(file_path).canonicalFilePath();

        const QList<QMdiSubWindow*> kSubWindows =
ui->mdiArea->subWindowList(); for (auto* window : kSubWindows) { auto*
editor_widget = qobject_cast<EntityEditor*>(window->widget());

                if (editor_widget) {
                        if (editor_widget->GetFileName() ==
absolute_path) { return window;
                        }
                }
        }
        return nullptr;
}

auto MainWindow::create_document_window(const QString& file_path)
    -> QMdiSubWindow*
{
        auto editor = new EntityEditor(ui->mdiArea, file_path);
        auto sub_window = ui->mdiArea->addSubWindow(editor);

        sub_window->setWindowTitle(file_path);
        return sub_window;
}
*/
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
