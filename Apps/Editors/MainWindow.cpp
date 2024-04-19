/* EditorWindow.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

static Ui::MainWindow ui_definition;

namespace ResourceEditor {
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui_definition.setupUi(this);

	connect(
	    ui_definition.actionOpen,
	    &QAction::triggered,
	    this,
	    &MainWindow::openFile
	);
}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
