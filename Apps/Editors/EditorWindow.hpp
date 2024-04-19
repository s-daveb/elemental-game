/* EditorWindow.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QMainWindow>
#include <QWidget>

namespace elemental::ResourceEditor {
class MainWindow : public QMainWindow {
	Q_OBJECT; // NOLINT
    public:
	MainWindow(QWidget* parent = nullptr): QMainWindow(parent) {

	}
};

}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen ::w
