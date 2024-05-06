/* MainWindow.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ui_MainWindow.h"

#include <QMainWindow>
#include <QObject>
#include <QString>
#include <QWidget>

#include <map>
#include <memory>

class QMdiSubWindow;
class QFileSystemModel;
class QModelIndex;
class QMdiSubWindow;
class QJsonModel;
class QFileInfo;

namespace ResourceEditor {
class MainWindow : public QMainWindow {
	// NOLINTNEXTLINE
	Q_OBJECT

	template<typename TData>
	using Ptr = std::unique_ptr<TData>;

	using FilePath = QString;

    public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

    protected slots:
	void onclick_menuaction_open_directory();
	void onclick_menuaction_new_file();

	void onclick_fstree_file(const QModelIndex& index);

    protected:
	void showEvent(QShowEvent*) override;

	void readDirectory(const QString& directory = "");
	QMdiSubWindow* loadFile(QFileInfo& fileInfo);

	QString current_directory;

	Ptr<Ui::MainWindow> ui;
	Ptr<QFileSystemModel> filesystem_model;
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
