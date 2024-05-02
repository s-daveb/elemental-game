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

#include "QJsonModel.hpp"

#include "IOCore/Exception.hpp"
#include "fmt/format.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

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
	this->read_directory(current_directory);
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
	auto current_directory = QDir::current().dirName();
	auto user_selection = QFileDialog::getExistingDirectory(
	    this,
	    QWidget::tr("Open Directory"),
	    current_directory,
	    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	this->read_directory(user_selection);
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
	    &MainWindow::onclick_fstree_file
	);
	current_directory = directory;
}

void MainWindow::onclick_fstree_file(const QModelIndex& index)
{
	auto path = filesystem_model->filePath(index);
	QFileInfo path_info(path);
	if (path_info.isDir()) {
		this->read_directory(path);
		return;
	}

	auto subwindow_list = this->ui->mdiArea->subWindowList();
	QMdiSubWindow* subwindow = nullptr;

	auto result = std::find_if(
	    subwindow_list.begin(),
	    subwindow_list.end(),
	    [&path](QWidget* widget_ptr) {
		    return (widget_ptr->objectName() == path);
	    }
	);

	if (result != subwindow_list.end()) {
		subwindow = *result;
		this->statusBar()->showMessage(
		    tr("Document was already loaded"), 1000
		);
	} else {
		auto filename = path_info.baseName();
		auto suffix = path_info.suffix();
		if (suffix == "json") {
			auto tree_widget = new QTreeView(this->ui->mdiArea);
			auto found = this->json_models.find(filename);
			QJsonModel* model = nullptr;

			if (found == this->json_models.end()) {
				this->json_models[filename] =
				    std::make_unique<QJsonModel>(this);
			}
			model = this->json_models.at(filename).get();
			tree_widget->setModel(model);

			QFile file(path);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream file_text(&file);

				model->loadJson(file_text.readAll().toUtf8());
				file.close();
			}

			subwindow = this->ui->mdiArea->addSubWindow(tree_widget);
			subwindow->setObjectName(path);
			subwindow->setWindowTitle(path_info.fileName());

		} else {
			this->statusBar()->showMessage(
			    tr("Opened New Document"), 1000
			);
			auto text_widget = new QPlainTextEdit(this->ui->mdiArea);

			QFile file(path);
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream file_text(&file);
				text_widget->setPlainText(file_text.readAll());
				file.close();
			}

			subwindow = this->ui->mdiArea->addSubWindow(text_widget);
			subwindow->setObjectName(path);
			subwindow->setWindowTitle(path_info.fileName());
		}
	}

	subwindow->show();
}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
