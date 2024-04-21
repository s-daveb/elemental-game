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

#include "ui_MainWindow.h"

class QMdiSubWindow;

namespace ResourceEditor {

class MainWindow : public QMainWindow {
	Q_OBJECT

    public:
	MainWindow(QWidget* parent = nullptr);
	virtual ~MainWindow();

    protected:
	virtual void showEvent(QShowEvent*) override;

    private slots:
	void open();

    private:
	void moveToCenter();

	void loadFile(const QString&);
	QMdiSubWindow* findDocumentWindow(const QString&);
	QMdiSubWindow* createDocumentWindow(const QString&);

	Ui::MainWindow& ui;

	QStringListModel component_list;
	// GameEngine::ComponentStore& component_store;
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
