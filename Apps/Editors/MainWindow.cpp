/* MainWindow.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <QObject>
#include <QWidget>

#include <QList>
#include <QListView>
#include <QStringListModel>

#include <QRect>

#include <QResizeEvent>

#include <QScreen>

#include <QApplication>
#include <QMainWindow>

#include <QMdiArea>
#include <QMdiSubWindow>

#include <QFileDialog>
#include <QStandardPaths>

#include <functional>
#include <iostream>

#include "IOCore/Exception.hpp"

#include "EntityEditor.hpp"
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

namespace ResourceEditor {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(*(new Ui::MainWindow()))
{
	ui.setupUi(this);

	connect(ui.actionOpen, SIGNAL(triggered(void)), this, SLOT(open(void)));

	//this->open();
}

MainWindow::~MainWindow()
{
	Ui::MainWindow* ui_def = &(this->ui);
	delete ui_def;
}

void MainWindow::showEvent(QShowEvent* event)
{
	this->moveToCenter();
}

void MainWindow::moveToCenter()
{
	auto screenRect = QApplication::primaryScreen()->availableGeometry();
	auto centerPoint = QPoint(
	    screenRect.x() + (screenRect.width() / 2),
	    screenRect.y() + (screenRect.height() / 2)
	);

	centerPoint.setX(centerPoint.x() - (this->width() / 2));
	centerPoint.setY(centerPoint.y() - (this->height() / 2));

	// this->move(centerPoint);
}

void MainWindow::open()
{
	QString filter = "JSON File (*.json)";
	QString file_name = QFileDialog::getOpenFileName(
	    (QWidget*)this, "open a file", QDir::current().dirName()
	);

	this->loadFile(file_name);
	/*	std::cout << "current selected_file:"
	                  << this->selected_file.toStdString() << std::endl;*/
}
void MainWindow::loadFile(const QString& file_path)
{
	QMdiSubWindow* document_to_open = findDocumentWindow(file_path);

	if (document_to_open) {
		statusBar()->showMessage(
		    tr("Document was already loaded"), 1000
		);
	} else {
		document_to_open = createDocumentWindow(file_path);

		if (document_to_open) {
			statusBar()->showMessage(tr("File loaded"), 1000);
		} else {
			throw IOCore::Exception("The file could not be loaded!");
		}
	}

	ui.mdiArea->setActiveSubWindow(document_to_open);
	document_to_open->show();
}

QMdiSubWindow* MainWindow::findDocumentWindow(const QString& file_path)
{
	QString absolute_path = QFileInfo(file_path).canonicalFilePath();

	const QList<QMdiSubWindow*> sub_windows = ui.mdiArea->subWindowList();
	for (auto* window : sub_windows) {
		auto* editor_widget =
		    qobject_cast<EntityEditor*>(window->widget());

		if (editor_widget) {
			if (editor_widget->GetFileName() == absolute_path) {
				return window;
			}
		}
	}
	return nullptr;
}

QMdiSubWindow* MainWindow::createDocumentWindow(const QString& file_path)
{
	EntityEditor* editor = new EntityEditor(ui.mdiArea, file_path);
	QMdiSubWindow* subWindow = ui.mdiArea->addSubWindow(editor);

	subWindow->setWindowTitle(file_path);
	return subWindow;
}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
