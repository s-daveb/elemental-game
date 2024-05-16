/* MainWindow.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "EditorWidgets/DocumentEditor.hpp"
#include "EditorWidgets/ExceptionDialog.hpp"
#include "EditorWidgets/JsonEditor.hpp"

#include "IOCore/Exception.hpp"

#include <fmt/format.h>

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
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

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

namespace ResourceEditor {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , filesystem_model(std::make_unique<QFileSystemModel>())
    , current_directory(QDir::homePath())

{
	this->ui->setupUi(this);

	connect(
	    this->ui->actionOpen,
	    &QAction::triggered,
	    this,
	    &MainWindow::onclick_menuaction_open_directory
	);
	this->readDirectory(current_directory);
	this->ui->fsTreeView->hideColumn(1);
	this->ui->fsTreeView->hideColumn(2);
	this->ui->fsTreeView->hideColumn(3);
	this->ui->fsTreeView->hideColumn(4);
	this->ui->fsTreeView->hideColumn(5);
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
void MainWindow::onclick_menuaction_open_directory()
{
	auto user_selection = QFileDialog::getExistingDirectory(
	    this,
	    QWidget::tr("Open Directory"),
	    this->current_directory,
	    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	this->readDirectory(user_selection);
};
void MainWindow::onclick_menuaction_new_file()
{
	auto current_directory = QDir::current().dirName();
	auto user_selection = QFileDialog::getExistingDirectory(
	    this,
	    QWidget::tr("Open Directory"),
	    current_directory,
	    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	this->readDirectory(user_selection);
};

void MainWindow::readDirectory(const QString& directory)
{
	auto model_index = this->filesystem_model->setRootPath(directory);

	this->ui->fsTreeView->setModel(this->filesystem_model.get());
	this->ui->fsTreeView->setRootIndex(model_index);

	/*
	this->ui->fsTreeView->hideColumn(1);
	this->ui->fsTreeView->hideColumn(2);
	this->ui->fsTreeView->hideColumn(3);
	this->ui->fsTreeView->hideColumn(4);
	this->ui->fsTreeView->hideColumn(5); */

	connect(
	    this->ui->fsTreeView,
	    &QTreeView::doubleClicked,
	    this,
	    &MainWindow::onclick_fstree_file
	);
	current_directory = directory;
}
auto MainWindow::loadFile(QFileInfo& fileInfo) -> QMdiSubWindow*
{
	QMdiSubWindow* subwindow_ptr = nullptr;
	try {
		auto path = fileInfo.filePath();
		auto filename = fileInfo.baseName();
		auto suffix = fileInfo.suffix();

		if (suffix == "json") {
			auto document_widget =
			    new DocumentEditor(this->ui->mdiArea, this, path);

			document_widget->loadFile<JsonEditor>(path);
			subwindow_ptr =
			    this->ui->mdiArea->addSubWindow(document_widget);
			subwindow_ptr->setObjectName(filename);
			subwindow_ptr->setWindowFilePath(path);
			subwindow_ptr->setWindowTitle(filename);
		} else {
			auto text_widget = new QPlainTextEdit(this->ui->mdiArea);

			QFile file(path);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream file_text(&file);
				text_widget->setPlainText(file_text.readAll());
				file.close();
			}

			subwindow_ptr =
			    this->ui->mdiArea->addSubWindow(text_widget);
			subwindow_ptr->setWindowFilePath(path);
			subwindow_ptr->setObjectName(path);
			subwindow_ptr->setWindowTitle(filename);
		}
	} catch (IOCore::Exception& e) {
		auto dialog = ExceptionDialog::display(this, e);
		subwindow_ptr = nullptr;
	}
	return subwindow_ptr;
}

void MainWindow::onclick_fstree_file(const QModelIndex& index)
{
	auto path = filesystem_model->filePath(index);
	auto path_info = QFileInfo(path);

	auto statusbar = this->statusBar();
	QMdiSubWindow* subwindow_ptr = nullptr;

	// Case 1:  User selected a directory. CD into it.
	if (path_info.isDir()) {
		this->readDirectory(path);
		return;
	}

	// Case 2: User selected a file we've already opened. Show it.
	auto subwindows = this->ui->mdiArea->subWindowList();
	auto result = std::find_if(
	    subwindows.begin(),
	    subwindows.end(),
	    [&path](QWidget* widgetPtr) {
		    return (widgetPtr->windowFilePath() == path);
	    }
	);
	if (result != subwindows.end()) {
		subwindow_ptr = *result;
		statusbar->showMessage(tr("Document was already loaded"), 2000);
	} else {
		// Case 3: User selected a new file. Open it.
		statusbar->showMessage(tr("Opening New Document"), 2000);

		subwindow_ptr = this->loadFile(path_info);

		ASSERT(subwindow_ptr != nullptr);
		subwindow_ptr->show();
	}
}

}
// clang-format off
// vim: set foldminlines=5 foldlevel=2 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
