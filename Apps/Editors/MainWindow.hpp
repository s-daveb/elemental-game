/* MainWindow.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QObject>
#include <QWidget>

#include <QMainWindow>
#include <QStringListModel>

#include <map>
#include <memory>

#include "ui_MainWindow.h"

class QMdiSubWindow;
class QFileSystemModel;
class QModelIndex;
class QMdiSubWindow;

namespace ResourceEditor {

class MainWindow : public QMainWindow {
	// NOLINTNEXTLINE
	Q_OBJECT
    public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

    protected slots:
	void onclick_open_directory();

    protected:
	void showEvent(QShowEvent*) override;
	void read_directory(const QString& directory);
	void on_file_click(const QModelIndex& index);

	std::unique_ptr<Ui::MainWindow> ui;
	std::unique_ptr<QFileSystemModel> filesystem_model;

	std::map<QString, QMdiSubWindow*> open_documents;
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
